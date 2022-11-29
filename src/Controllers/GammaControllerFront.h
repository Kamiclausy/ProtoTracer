#include <Arduino.h>
#include <OctoWS2811.h>

#include "Controller.h"
#include "Render/Camera.h"
#include "Flash/PixelGroups/KaiborgV1Pixels.h"

const int ledsPerStrip = 346;
DMAMEM int displayMemory[346 * 6];
int drawingMemory[346 * 6];
const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

class GammaControllerFront : public Controller {
private:
    CameraLayout cameraLayout = CameraLayout(CameraLayout::ZForward, CameraLayout::YUp);

    Transform camTopTransform = Transform(Vector3D(0.0f, 0.0f, -139.06f), Vector3D(46.445f, 130.121f, -500.0f), Vector3D(1, 1, 1));
    Transform camBotTransform = Transform(Vector3D(0.0f, 180.0f, 0.0f), Vector3D(0, 0, -500.0f), Vector3D(1, 1, 1));

    PixelGroup camTopPixels = PixelGroup(KaiborgV1Pixels, 571, PixelGroup::ZEROTOMAX);
    PixelGroup camBotPixels = PixelGroup(KaiborgV1Pixels, 571, PixelGroup::MAXTOZERO);

    Camera camTop = Camera(&camTopTransform, &cameraLayout, &camTopPixels);
    Camera camBot = Camera(&camBotTransform, &cameraLayout, &camBotPixels);

    Camera* cameras[2] = { &camTop, &camBot };

public:
    GammaControllerFront(uint8_t maxBrightness) : Controller(cameras, 2, maxBrightness, 0){}

    void Initialize() override{
        leds.begin();
        leds.show();
    }

    void Display() override {
        int offset;

        for (int i = 0; i < 571; i++){
            camBotPixels.GetPixel(i)->Color = camBotPixels.GetPixel(i)->Color.Scale(brightness);
            camTopPixels.GetPixel(i)->Color = camTopPixels.GetPixel(i)->Color.Scale(brightness);
        }

        //0: BLT, RIGHT
        //1: FLT, RIGHT
        //2: BRT, LEFT
        //3: FRT, LEFT
        //4: BRB, RIGHT
        //5: FRB, RIGHT
        //6: BLB, LEFT
        //7: FLB, LEFT

        for (int i = 0; i < 571; i++) {
            if (i < 346){
                offset = i + 225;
                leds.setPixel(i + 346 * 0, camTopPixels.GetPixel(i)->Color.R, camTopPixels.GetPixel(i)->Color.G, camTopPixels.GetPixel(i)->Color.B);//Top Right
                leds.setPixel(570 - i + 346 * 4, camBotPixels.GetPixel(i)->Color.R, camBotPixels.GetPixel(i)->Color.G, camBotPixels.GetPixel(i)->Color.B);//Bottom Right

                leds.setPixel(i + 346 * 2, camTopPixels.GetPixel(570 - offset)->Color.R, camTopPixels.GetPixel(570 - offset)->Color.G, camTopPixels.GetPixel(570 - offset)->Color.B);//Top Left
                leds.setPixel(i + 346 * 6, camBotPixels.GetPixel(offset)->Color.R, camBotPixels.GetPixel(offset)->Color.G, camBotPixels.GetPixel(offset)->Color.B);//Bottom Left
            }
            else{
                offset = i - 346;
                leds.setPixel(i + 346 * 1 - 346, camTopPixels.GetPixel(i)->Color.R, camTopPixels.GetPixel(i)->Color.G, camTopPixels.GetPixel(i)->Color.B);//Top Right
                leds.setPixel(570 - i + 346 * 5 - 346, camBotPixels.GetPixel(i)->Color.R, camBotPixels.GetPixel(i)->Color.G, camBotPixels.GetPixel(i)->Color.B);//Bottom Right

                leds.setPixel(i + 346 * 3 - 346, camTopPixels.GetPixel(570 - offset)->Color.R, camTopPixels.GetPixel(570 - offset)->Color.G, camTopPixels.GetPixel(570 - offset)->Color.B);//Top Left
                leds.setPixel(i + 346 * 7 - 346, camBotPixels.GetPixel(offset)->Color.R, camBotPixels.GetPixel(offset)->Color.G, camBotPixels.GetPixel(offset)->Color.B);//Bottom Left
            }
        }
        
        leds.show();
    }

    void SetPixels(uint8_t strip, uint16_t led, RGBColor rgb){
        leds.setPixel(346 * strip + led, rgb.R, rgb.G, rgb.B);//Pin 8
    }
};
