#include <uvc_thread.h>

UVCThread::UVCThread(QObject *parent) :
    QThread(parent)
{
    running = false;
}

UVCThread::~UVCThread()
{

}

void UVCThread::cb(uvc_frame_t *frame, void *ptr) {

  uvc_frame_t *rgb_Frame;
  uvc_error_t error;
  //allocate RGB frame
  rgb_Frame = uvc_allocate_frame(frame->width * frame->height * 3);
  if (!rgb_Frame) {
    printf("unable to allocate bgr frame!");
    return;
  }
  //Convert to RGB
  error = uvc_any2rgb(frame, rgb_Frame);
  if (error) {
    uvc_perror(error, "uvc_any2bgr");
    uvc_free_frame(rgb_Frame);
    return;
  }

  //Get UVCThread object
  UVCThread *preview = reinterpret_cast<UVCThread*>(ptr);
  //Convert frame to QImage
  QImage image = QImage((uchar *) rgb_Frame->data, rgb_Frame->width, rgb_Frame->height, QImage::Format_RGB888);
  //Emit QImage as a signal
  emit preview->captureImage(image);
  //Free conversion frame memory
  uvc_free_frame(rgb_Frame);
}

void UVCThread::run()
{
  uvc_context_t *ctx;
  uvc_device_t *dev;
  uvc_device_handle_t *devh;
  uvc_stream_ctrl_t ctrl;
  uvc_error_t res;

  /* Initialize a UVC service context. Libuvc will set up its own libusb
   * context. Replace NULL with a libusb_context pointer to run libuvc
   * from an existing libusb context. */
  res = uvc_init(&ctx, NULL);
  if (res < 0) {
    uvc_perror(res, "uvc_init");
    return;
  }
  puts("UVC initialized");
  /* Locates the first attached UVC device, stores in dev */
  res = uvc_find_device(
      ctx, &dev,
      0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */
  if (res < 0) {
    uvc_perror(res, "uvc_find_device"); /* no devices found */
  } else {
    puts("Device found");
    /* Try to open the device: requires exclusive access */
    res = uvc_open(dev, &devh);
    if (res < 0) {
      uvc_perror(res, "uvc_open"); /* unable to open device */
    } else {
      puts("Device opened");
      /* Print out a message containing all the information that libuvc
       * knows about the device */
      uvc_print_diag(devh, stderr);
      /* Try to negotiate a 640x480 30 fps YUYV stream profile */
      res = uvc_get_stream_ctrl_format_size(
          devh, &ctrl, /* result stored in ctrl */
          UVC_COLOR_FORMAT_UNCOMPRESSED, /* YUV 422, aka YUV 4:2:2. try _COMPRESSED */
          1920, 1080, 30 /* width, height, fps */
      );
      /* Print out the result */
      uvc_print_stream_ctrl(&ctrl, stderr);
      if (res < 0) {
        uvc_perror(res, "get_mode"); /* device doesn't provide a matching stream */
      } else {
        /* Start the video stream. The library will call user function cb:
         *   cb(frame, (void*) 12345)
         */
        res = uvc_start_streaming(devh, &ctrl, cb, this, 0);
        if (res < 0) {
          uvc_perror(res, "start_streaming"); /* unable to start stream */
        } else {
          puts("Streaming...");
        //  uvc_set_ae_mode(devh, 1); /* e.g., turn on auto exposure */

          sleep(10000); /* stream for 10 seconds */
          /* End the stream. Blocks until last callback is serviced */
          uvc_stop_streaming(devh);
          puts("Done streaming.");
        }
      }
      /* Release our handle on the device */
      uvc_close(devh);
      puts("Device closed");
    }
    /* Release the device descriptor */
    uvc_unref_device(dev);
  }
  /* Close the UVC context. This closes and cleans up any existing device handles,
   * and it closes the libusb context if one was not provided. */
  uvc_exit(ctx);
  puts("UVC exited");
}
