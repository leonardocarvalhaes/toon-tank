#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();

	bIsPlayerAlive = false;

	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ExplosionShake);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

bool APawnTank::GetIsPlayerAlive()
{
	return bIsPlayerAlive;
}

void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetIsPlayerAlive())
	{
		Move();
		Rotate();

		if(PlayerController)
		{
			FHitResult TraceHitResult;
			PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
			FVector HitLocation = TraceHitResult.ImpactPoint;

			RotateTurret(HitLocation);
		}
	}
}

void APawnTank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Value)
{
	MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value)
{
	float RotateAmount = Value * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);
	RotationDirection = FQuat(Rotation);
}

void APawnTank::Fire()
{
	if(GetIsPlayerAlive()) Super::Fire();
}

void APawnTank::Move()
{
	AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
}
