// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// scale
	{
		float CurrentScale = OurVisibleComponent->GetComponentScale().X;

		if (!CurrentVelocity.IsZero()) {
			if (bGrowing) {
				CurrentScale += 2;
			}
		}

		if (bGrowing) {
			CurrentScale += DeltaTime;
		}
		else {
			CurrentScale -= (DeltaTime * 4);
		}
		CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
		OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));

	}

	// location
	{
		TimeSpeedIncrease -= DeltaTime;
		if (!CurrentVelocity.IsZero()) {
			TimeSpeedIncrease += DeltaTime * 2;
		}
		else {
			TimeSpeedIncrease = 0.0f;
		}

		if (TimeSpeedIncrease >= 2.0f) {  // increase speed after held buttom for 2 seconds
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Speed up!"));
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime*4);  
			SetActorLocation(NewLocation);
		}
		else {
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);  
			SetActorLocation(NewLocation);
		}
	}

	// rotation
	{
		if (!CurrentRotation.IsZero()) {
			FRotator NewRotation = GetActorRotation() + (CurrentRotation * DeltaTime);
			SetActorRotation(NewRotation);
		}
	}

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Respond when our "Grow" key is pressed or released.
	InputComponent->BindAction("Grow/ParticleToggle", IE_Pressed, this, &AMyPawn::StartGrowing);
	InputComponent->BindAction("Grow/ParticleToggle", IE_Released, this, &AMyPawn::StopGrowing);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("MoveX", this, &AMyPawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AMyPawn::Move_YAxis);

	//Rotate Y
	InputComponent->BindAxis("RotateY", this, &AMyPawn::RotateY);
}

void AMyPawn::Move_XAxis(float AxisValue)
{   
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100;    // Move at 100 units per second
}

void AMyPawn::Move_YAxis(float AxisValue)
{
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100;    // Move at 100 units per second
}

void AMyPawn::RotateY(float AxisValue)
{
	CurrentRotation.Yaw = AxisValue * 100;
}

void AMyPawn::StartGrowing()
{
	bGrowing = true;
}

void AMyPawn::StopGrowing()
{
	bGrowing = false;
}

