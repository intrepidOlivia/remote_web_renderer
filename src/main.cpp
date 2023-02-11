#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>

using namespace ultralight;

const char* getHtml();

class MyWebRenderer : public LoadListener {
        RefPtr<Renderer> renderer_;
        RefPtr<View> view_;
        bool done_;
    public:
        MyWebRenderer() {
            // Set up Platform
            Config config;
            config.device_scale = 2.0;
            config.font_family_standard = "Arial";
            config.use_gpu_renderer = false;
            Platform::instance().set_config(config);
            Platform::instance().set_font_loader(GetPlatformFontLoader());
            Platform::instance().set_file_system(GetPlatformFileSystem("."));

            // Set up Renderer
            renderer_ = Renderer::Create();

            // Set up View
            view_ = renderer_->CreateView(800, 55, false, nullptr);
            view_->set_load_listener(this);
            view_->LoadHTML(getHtml());
        }

        virtual ~MyWebRenderer() {
            view_ = nullptr;
            renderer_ = nullptr;
        }

        virtual void OnFinishLoading(ultralight::View* caller,
            uint64_t frame_id, bool is_main_frame, const String& url) override {
            ///
            /// Our page is done when the main frame finishes loading.
            ///
            if (is_main_frame) {
            std::cout << "Our page has loaded!" << std::endl;

            ///
            /// Set our done flag to true to exit the Run loop.
            ///
            done_ = true;
            }
        }

        void MyWebRenderer::drawToBitmap() {
            while (!done_) {
                std::this_thread::sleep_for (std::chrono::milliseconds(10));
                renderer_->Update();
                renderer_->Render();
            }

            BitmapSurface* bitmap_surface = (BitmapSurface*)view_->surface();
            RefPtr<Bitmap> bitmap = bitmap_surface->bitmap();
            bitmap->SwapRedBlueChannels();
            bitmap->WritePNG("result.png");
        }
};

int main()
{
    MyWebRenderer myWebRenderer;
    myWebRenderer.drawToBitmap();
    return 0;
}

const char* getHtml() {
    return R"(
        <html>
          <body>
            <div>
              Hello world. I am a PNG.
            </div>
          </body>
        </html>
    )";
}


