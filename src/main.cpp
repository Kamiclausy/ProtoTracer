//#define RIGHTFACE
//#define DEMOMODE


//--------------- ANIMATIONS ---------------
//#include "Animation\ProtoDRMorphAnimation.h"
//#include "Animation\ProtogenKitFaceAnimation.h"
//#include "Animation\ProtogenHUB75Animation.h"
#include "Animation\ProtogenHUB75AnimationSplit.h"
//#include "Animation\Commissions\VesperAnimation.h"
//#include "Animation\Commissions\TechSaneAnimation.h"
//#include "Animation\KaiborgV1Animation.h"
//#include "Animation\ProtoV3Animation.h"
//#include "Animation\FullScreenAnimation.h"
//#include "Animation\VectorFieldAnimation.h"
//#include "Animation\CoelaBonkAnimation.h"
//#include "Animation\FoxAnimation.h"
//#include "Animation\SpyroAnimation.h"
//#include "Animation\SpyroRotateAnimation.h"
//#include "Animation\PikachuAnimation.h"
//#include "Animation\BeeAnimation.h"
//#include "Animation\ProtogenArtleckAnimation.h"
//#include "Animation\DeltaruneAnimation.h"
//#include "Animation\NukudeFaceAnimation.h"
//#include "Animation\Commissions\InfraredAnimation.h"

//--------------- CONTROLLERS ---------------
//#include "Controllers\KaiborgV1Controller.h"
//#include "Controllers\KaiborgV1D1Controller.h"
//#include "Controllers\ProtoDRController.h"
//#include "Controllers\SmartMatrixHUB75.h"
#include "Controllers\SmartMatrixHUB75Split.h"

uint8_t maxBrightness = 50;
#ifdef RIGHTFACE
ProtoDRController controller = ProtoDRController(maxBrightness, ProtoDRController::RIGHT);
#else
//ProtoDRController controller = ProtoDRController(maxBrightness, ProtoDRController::LEFT);
#endif
SmartMatrixHUB75Split controller = SmartMatrixHUB75Split(maxBrightness);
//SmartMatrixHUB75 controller = SmartMatrixHUB75(maxBrightness);
ProtogenHUB75AnimationSplit animation = ProtogenHUB75AnimationSplit();

void setup() {
    Serial.begin(115200);
    Serial.println("\nStarting...");

    controller.Initialize();
}

void loop() {
    float ratio = (float)(millis() % 5000) / 5000.0f;

    controller.SetBrightness(Menu::GetBrightness());// / 8);

    //animation.UpdateTime(ratio);
    //controller.Render(animation.GetScene());
    
    animation.SetCameraMirror(false);
    animation.UpdateTime(ratio);
    controller.RenderCamera(animation.GetScene(), 0);

    animation.SetCameraMirror(true);
    animation.UpdateTime(ratio);
    controller.RenderCamera(animation.GetScene(), 1);
    

    controller.Display();
    
    Serial.print("Animated in ");
    Serial.print(animation.GetAnimationTime(), 4);

    Serial.print("s, Rendered in ");
    Serial.print(controller.GetRenderTime(), 4);
    Serial.println("s");
}
