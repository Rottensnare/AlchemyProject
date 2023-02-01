// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "AI/AIBase.h"
#include "Alchemy/Potion.h"
#include "Camera/CameraComponent.h"
#include "Components/AlchemyComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/UserDefinedStruct.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Pickable.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "PlayerController/MyPlayerController.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	AlchemyComponent = CreateDefaultSubobject<UAlchemyComponent>(TEXT("AlchemyComponent"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSourceComp"));
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Hearing::StaticClass());
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<AMyPlayerController>(Controller);

	if(InventoryComponent) InventoryComponent->Character = this;
	if(AlchemyComponent) AlchemyComponent->Character = this;
	
	GetWorldTimerManager().SetTimer(HUDInitTimer, this, &APlayerCharacter::HUDInitTimerFinished, HUDInitTime);
	
	HeadSocket = GetMesh()->GetSocketByName("headSocket");

	if(GetCharacterMovement()) DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
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
	if(TracedActor->Implements<UInteractable>())
	{
		IInteractable* Interactable = Cast<IInteractable>(TracedActor);
		if(Interactable->Interact(this))
		{
			if(AAIBase* TempAI = Cast<AAIBase>(TracedActor))
			{
				
				MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(GetController()) : MyPlayerController;
				if(MyPlayerController)
				{
					CurrentNPC = TempAI;
					CurrentNPC_ID = TempAI->NPC_ID;
					MyPlayerController->ToggleDialogueOverlay();
					
				}
			}
		}
	}
	else if(TracedActor->Implements<UPickable>())
	{
		if(InventoryComponent == nullptr) return;
		
		if(APotion* TempPotion = Cast<APotion>(TracedActor)) //TODO: Use interface functions
		{
			const uint32 HashCode = UUserDefinedStruct::GetUserDefinedStructTypeHash(&TempPotion->ProductInfo, FProductInfo::StaticStruct());
			InventoryComponent->AddPotionToInventory(TempPotion, 1, HashCode);
			TracedActor = nullptr;
			MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(GetController()) : MyPlayerController;
			if(MyPlayerController) MyPlayerController->PlaySound(FName("Pop"));
			return;
		}
		AItem* TempItem = Cast<AItem>(TracedActor);
		if(TempItem)
		{
			InventoryComponent->AddToInventory(TempItem, 1);
			TracedActor = nullptr;
			MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(GetController()) : MyPlayerController;
			if(MyPlayerController) MyPlayerController->PlaySound(FName("Pop"));
		}
	}
}

void APlayerCharacter::SweepInteractButtonPressed()
{
	if(TracedActor == nullptr) return;
	if(TracedActor->Implements<UPickable>())
	{
		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMulti(
			this,
			GetActorLocation(),
			GetActorLocation() + FVector(0.f, 0.f, 0.1f),
			SweepRadius,
			 UEngineTypes::ConvertToTraceType(ECC_Visibility),
			 false,
			 TArray<AActor*>(),
			 EDrawDebugTrace::ForDuration,
			 HitResults,
			 true,
			 FLinearColor::Green,
			 FLinearColor::Red,
			 3.f);

		TArray<AItem*> Items; //BUG: This won't work for potions or other types that have class specific variables that are needed.
		for(auto HitResult : HitResults)
		{
			if(HitResult.GetActor()->GetClass() == TracedActor->GetClass())
			{
				AItem* TempItem = Cast<AItem>(HitResult.GetActor());
				if(TempItem)
				{
					Items.AddUnique(TempItem);
				}
			}
		}
		
		MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(GetController()) : MyPlayerController;
		if(MyPlayerController) MyPlayerController->PlaySound(FName("Pop"));
		
		if(Items.Num() > 0)
		{
			InventoryComponent->AddToInventory(Items[0], Items.Num());
			for(const auto& TempItem : Items)
			{
				TempItem->Destroy();
			}
		}
		TracedActor = nullptr;
		
	}
	
}

void APlayerCharacter::TraceForObjects()
{
	if(HeadSocket == nullptr) return;
	FHitResult HitResult;
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
		FVector Start{TraceWorldPosition};
		float DistanceToHead = (HeadSocket->GetSocketLocation(GetMesh()) - Start).Size();
		Start += TraceWorldDirection * (DistanceToHead);
		const FVector End{Start + TraceWorldDirection * ObjectTraceRadius};
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(HitResult, CameraComponent->GetComponentLocation(), End, ECC_Visibility, QueryParams);
		if(HitResult.bBlockingHit)
		{
			
			if(AItem* TempItem = Cast<AItem>(HitResult.GetActor()))
			{
				if(TempItem != TracedItemLastFrame)
				{
					TempItem->ShowItemPopupWidget();
					if(TracedItemLastFrame) TracedItemLastFrame->ShowItemPopupWidget();
					TracedItemLastFrame = TempItem;
				}
			}
			else
			{
				if(TracedItemLastFrame) TracedItemLastFrame->ShowItemPopupWidget();
				TracedItemLastFrame = nullptr;
			}
			TracedActor = HitResult.GetActor();
		}

			
		else TracedActor = nullptr;
	}
}

void APlayerCharacter::ShowInfoButtonPressed() //TODO: Can be split into smaller functions. Some of the code here is repeated
{
	if(HeadSocket == nullptr) return;
	FHitResult HitResult;
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
		FVector Start{TraceWorldPosition};
		float DistanceToHead = (HeadSocket->GetSocketLocation(GetMesh()) - Start).Size();
		Start += TraceWorldDirection * (DistanceToHead);
		const FVector End{Start + TraceWorldDirection * DebugObjectTraceRadius};
		UKismetSystemLibrary::BoxTraceSingle(
			this,
			Start,
			End,
			FVector(10.f),
			FRotator::ZeroRotator,
			UEngineTypes::ConvertToTraceType(ECC_Pawn),
			false,
			 TArray<AActor*>(),
			 EDrawDebugTrace::ForDuration,
			 HitResult,
			 true,
			 FLinearColor::Green,
			 FLinearColor::Red,
			 3.f);
		if(HitResult.bBlockingHit)
		{
			AAIBase* TempAIBase = Cast<AAIBase>(HitResult.GetActor());
			if(TempAIBase)
			{
				if(ABaseAIController* BaseAIController = Cast<ABaseAIController>(TempAIBase->GetController()))
				{
					BaseAIController->ShowAIInfo();
				}
			}
		}
	}
}

void APlayerCharacter::ToggleSprint()
{
	if(GetCharacterMovement() == nullptr) return;
	
	if(bSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		bSprinting = false;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		bSprinting = true;
	}
	
	
}

void APlayerCharacter::EndInteraction()
{
	UE_LOG(LogTemp, Warning, TEXT("EndInteraction"))
	if(bIsConversing)
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsConversing"))
		MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(Controller) : MyPlayerController;
		if(MyPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("MyPlayerController"))
			MyPlayerController->ToggleDialogueOverlay();
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
	PlayerInputComponent->BindAction("ShowInfo", IE_Pressed, this, &APlayerCharacter::ShowInfoButtonPressed);
	PlayerInputComponent->BindAction("ToggleSprint", IE_Pressed, this, &APlayerCharacter::ToggleSprint);
	PlayerInputComponent->BindAction("EndInteraction", IE_Pressed, this, &APlayerCharacter::EndInteraction);
	
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

void APlayerCharacter::UsePotion(const TSubclassOf<UPotionComponent> InComponentClass, const EProductQuality InProductQuality)
{
	if(InComponentClass == nullptr) return;

	auto* PotionComponent = NewObject<UPotionComponent>(this, InComponentClass, NAME_None, RF_Transient);
	if(PotionComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create PotionComponent"))
		return;
	}
	PotionComponent->RegisterComponent();
	PotionComponent->SetProductQuality(InProductQuality);
	CurrentPotionComponents.Emplace(PotionComponent, 0);
}
