// Fill out your copyright notice in the Description page of Project Settings.
#include "FloatingActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (CubeVisualAsset.Succeeded()) {
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);     //set object type
		VisualMesh->SetRelativeLocation(FVector::ZeroVector);  //set location
	}

}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFloatingActor::ActorMovementAtoB(FVector StartPosition, FVector EndPosition, float DeltaTime, float MovementSpeed)
{
	FVector CurrentLocation = GetActorLocation();
	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Cos(RunningTime + DeltaTime) - FMath::Cos(RunningTime));
	CurrentLocation.Z += DeltaHeight * FloatSpeed;

	if (PositionFlag == true) {
		CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, EndPosition, DeltaTime, MovementSpeed);
		if (CurrentLocation == EndPosition) {
			PositionFlag = false;
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("End"));
		}
	}
	else {
		CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, StartPosition, DeltaTime, MovementSpeed);
		if (CurrentLocation == StartPosition) {
			PositionFlag = true;
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Start"));
		}
	}

	SetActorLocation(CurrentLocation);
}

void AFloatingActor::ActorRotatorAtoB(FVector StartPosition, FVector EndPosition, float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentRotator = GetActorRotation();
	float RunningTime = GetGameTimeSinceCreation();
	float DeltaRotation = DeltaTime * FloatRotation;

	if (PositionFlag == true) {
		CurrentRotator.Yaw += DeltaRotation;
		if (CurrentLocation == EndPosition) {
			CurrentRotator = FRotator::ZeroRotator;
		}
	}
	else {
		CurrentRotator.Yaw += -DeltaRotation;
		if (CurrentLocation == StartPosition) {
			CurrentRotator = FRotator::ZeroRotator;
		}
	}
    
	SetActorRotation(CurrentRotator);
}

void AFloatingActor::DetectObjects()
{
	FHitResult NearResult;
	FVector StartPosition = GetActorLocation();
	float Lenghts = 200.0f;
	FVector EndPosition = StartPosition + Lenghts * -GetActorUpVector(); //GetActorForwardVector();
	TArray<FColor> ColorArray = { FColor::Green, FColor::Red };


	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);


	if (DetectionFlag) {
		DrawDebugLine(GetWorld(), StartPosition, EndPosition, ColorArray[1], false, 0.0f, 0.0f, 4.0f);
	}
	else {
		DrawDebugLine(GetWorld(), StartPosition, EndPosition, ColorArray[0], false, 0.0f, 0.0f, 4.0f);
	}

	if (GetWorld()->LineTraceSingleByChannel(NearResult, StartPosition, EndPosition, ECC_Visibility, QueryParams)) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("Near you"));
		DetectionFlag = true;
	}
	else {
		DetectionFlag = false;
	}


}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	//OurPlayerController->SetViewTarget(CameraFollow);

	//FVector CurrentLocation = GetActorLocation();
	StartPoint = FVector(-180.0f, 0.0f, 300.0f);
	EndPoint = FVector(300.f, 0.0, 300.0f);
	float speed{ 160.0f };

	ActorMovementAtoB(StartPoint, EndPoint, DeltaTime, speed);
	ActorRotatorAtoB(StartPoint, EndPoint, DeltaTime);
	DetectObjects();

}

void AFloatingActor::ObjectIterator()
{
	for (TObjectIterator<UObject> it; it; ++it)
	{
		UObject* CurrentObject = *it;
		UE_LOG(LogTemp, Log, TEXT("Found UObject named: %s"), *CurrentObject->GetName());
	}
	
}


