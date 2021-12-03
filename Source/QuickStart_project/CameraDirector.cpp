// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraDirector.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraDirector::ACameraDirector()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraDirector::BeginPlay()
{
	Super::BeginPlay();
	
}




// Called every frame
void ACameraDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float SmoothBlendTime = 0.75f;

	//TimeToNextCameraChange -= DeltaTime;
	//if (TimeToNextCameraChange <= 0.0f) {
	//	if (ItTroughCameras == CamerasStruct.Num()) {
	//		ItTroughCameras = 0;
	//	}
	//	TimeToNextCameraChange += CamerasStruct[ItTroughCameras].TimeBetweenCameraChanges;
	//	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this,0);
	//	if (OurPlayerController) {
	//		OurPlayerController->SetViewTargetWithBlend(CamerasStruct[ItTroughCameras].Camera, SmoothBlendTime);
	//		ItTroughCameras++;
	//	}
	//

}

