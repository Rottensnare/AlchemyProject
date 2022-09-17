// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Pickable.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/MyPlayerController.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<AMyPlayerController>(Controller);

	if(InventoryComponent)
	{
		InventoryComponent->Character = this;
	}

	GetWorldTimerManager().SetTimer(HUDInitTimer, this, &APlayerCharacter::HUDInitTimerFinished, HUDInitTime);
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForObjects();
}

void APlayerCharacter::MoveForward(float Value)
{
	if(Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if(Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * MouseXSensitivity);
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * MouseYSensitivity);
}

void APlayerCharacter::InventoryButtonPressed()
{
	if(bInventoryOpen)
	{
		MyPlayerController->SetShowMouseCursor(false);
		
		MyPlayerController->SetInputMode(InputModeGameOnly);
		InventoryComponent->ShowInventory(false);
		bInventoryOpen = !bInventoryOpen;
	}
	else
	{
		InputModeGameAndUI.SetHideCursorDuringCapture(false);
		MyPlayerController->SetShowMouseCursor(true);
		InventoryComponent->ShowInventory(true);
		MyPlayerController->SetInputMode(InputModeGameAndUI);
		bInventoryOpen = !bInventoryOpen;
	}
}

void APlayerCharacter::InventoryButtonReleased()
{
	
}

void APlayerCharacter::InteractButtonPressed()
{
	if(TracedActor == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("Traced Actor: %s"), *TracedActor->GetName())
	if(TracedActor->Implements<UPickable>())
	{
		AItem* TempItem = Cast<AItem>(TracedActor);
		if(TempItem && InventoryComponent)
		{
			InventoryComponent->AddToInventory(TempItem, 1);
			TracedActor = nullptr;
		}
	}
}

void APlayerCharacter::SweepInteractButtonPressed()
{
	
}

void APlayerCharacter::TraceForObjects()
{
	FHitResult HitResult;
	FVector HitLocation;
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D ScreenCenter = FVector2D(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector TraceWorldPosition, TraceWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), ScreenCenter, TraceWorldPosition, TraceWorldDirection);
	if(bScreenToWorld)
	{
		const FVector Start{TraceWorldPosition};
		const FVector End{Start + TraceWorldDirection * ObjectTraceRadius};
		HitLocation = End;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
		if(HitResult.bBlockingHit)
		{
			//DrawDebugCircle(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Red, false, 2.f);
			TracedActor = HitResult.GetActor();
		}
		else
		{
			TracedActor = nullptr;
		}
	}
}

void APlayerCharacter::HUDInitTimerFinished()
{
	MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(Controller) : MyPlayerController;
	if(MyPlayerController)
	{
		MyPlayerController->SetHUDHealth(HealthComponent->Health, HealthComponent->MaxHealth);
		if(InventoryComponent)
		{
			MyPlayerController->SetInventoryGrid(InventoryComponent->NumberOfInventorySlots);
		}
	}
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &APlayerCharacter::InventoryButtonPressed);
	PlayerInputComponent->BindAction("ToggleInventory", IE_Released, this, &APlayerCharacter::InventoryButtonReleased);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::InteractButtonPressed);
	PlayerInputComponent->BindAction("SweepInteract", IE_Pressed, this, &APlayerCharacter::SweepInteractButtonPressed);

	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::UpdateInventorySlotAmount(const int32 Index, const int32 Amount)
{
	InventoryComponent->UpdateItemAmount(Index, Amount);
}

