// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "AlchemyProject/Ingredient.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/Alchemy/AlchemyOverlay.h"
#include "AlchemyProject/Alchemy/IngredientData.h"
#include "AlchemyProject/Alchemy/UI/AlchemyScrollBox.h"
#include "AlchemyProject/Components/AlchemyComponent.h"
#include "AlchemyProject/Enums/CustomDataTables.h"
#include "AlchemyProject/HUD/InventorySlot.h"
#include "AlchemyProject/HUD/InventoryWidget.h"
#include "AlchemyProject/HUD/PlayerHUD.h"
#include "AlchemyProject/HUD/PlayerOverlay.h"
#include "AlchemyProject/HUD/ScrollableInventoryWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "HUD/DialogueBox.h"
#include "HUD/DialogueOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/DialogueManager.h"
#include "Sound/SoundCue.h"

AMyPlayerController::AMyPlayerController()
{
	PawnNoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComponent"));
}

void AMyPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD == nullptr) return;

	bool bIsHUDValid =	PlayerHUD->PlayerOverlay &&
						PlayerHUD->PlayerOverlay->HealthBar &&
						PlayerHUD->PlayerOverlay->HealthText;

	if(bIsHUDValid)
	{
		PlayerHUD->PlayerOverlay->HealthBar->SetPercent(Health / MaxHealth);
		const FText HealthTxt = FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::TruncToInt32(Health), FMath::TruncToInt32(MaxHealth)));
		PlayerHUD->PlayerOverlay->HealthText->SetText(HealthTxt);
	}
}

void AMyPlayerController::SetInventoryVisibility(bool bVisible)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD == nullptr) return;

	bool bIsHUDValid =	PlayerHUD->PlayerOverlay &&
						PlayerHUD->PlayerOverlay->InventoryWidget &&
							PlayerHUD->PlayerOverlay->ScrollableInventoryWidget;
	if(bIsHUDValid)
	{
		if(bVisible)
		{
			if(bUseScrollableInventory)
			{
				PlayerHUD->PlayerOverlay->ScrollableInventoryWidget->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				PlayerHUD->PlayerOverlay->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			if(bUseScrollableInventory)
			{
				PlayerHUD->PlayerOverlay->ScrollableInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				PlayerHUD->PlayerOverlay->InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	
}

void AMyPlayerController::SetInventoryGrid(int32 NumberOfSlots)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD == nullptr) return;

	bool bIsHUDValid =	PlayerHUD->AlchemyOverlay &&
						PlayerHUD->PlayerOverlay &&
						PlayerHUD->PlayerOverlay->InventoryWidget &&
						PlayerHUD->PlayerOverlay->ScrollableInventoryWidget;
	if(bIsHUDValid)
	{
		if(bUseScrollableInventory)
		{
			PlayerHUD->PlayerOverlay->ScrollableInventoryWidget->CreateInventory(NumberOfSlots);
		}
		else
		{
			PlayerHUD->PlayerOverlay->InventoryWidget->CreateInventoryGrid(NumberOfSlots);
			PlayerHUD->AlchemyOverlay->CharacterInventory->CreateInventoryGrid(NumberOfSlots);
		}
		
	}
}

void AMyPlayerController::UpdateInventory(const int32 Index)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD && PlayerHUD->PlayerOverlay && PlayerHUD->PlayerOverlay->InventoryWidget && PlayerHUD->AlchemyOverlay && PlayerHUD->AlchemyOverlay->CharacterInventory)
	{
		PlayerHUD->PlayerOverlay->InventoryWidget->UpdateSlotFromInventory(Index);
		
	}
}

void AMyPlayerController::ToggleAlchemyOverlay()
{
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetCharacter()) : CurrentCharacter;
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD && PlayerHUD->AlchemyOverlay && PlayerHUD->AlchemyOverlay->CharacterInventory && PlayerHUD->PlayerOverlay && CurrentCharacter)
	{
		if(PlayerHUD->AlchemyOverlay->GetVisibility() == ESlateVisibility::Collapsed)
		{
			PlayerHUD->AlchemyOverlay->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->PlayerOverlay->SetVisibility(ESlateVisibility::Collapsed);
			FInputModeGameAndUI InputModeGameAndUI;
			SetInputMode(InputModeGameAndUI);
			SetShowMouseCursor(true);
			PlayerHUD->AlchemyOverlay->CharacterInventory->UpdateAllSlots();
			PlayerHUD->PlayerOverlay->InventoryWidget->UpdateAllSlots();
			PlayerHUD->AlchemyOverlay->AlchemyScrollBox->UpdateInfo(CurrentCharacter->GetAlchemyComponent()->GetKnownRecipes());
			CurrentCharacter->bIsDoingAlchemy = true;
			
		}
		else
		{
			PlayerHUD->AlchemyOverlay->SetVisibility(ESlateVisibility::Collapsed);
			PlayerHUD->PlayerOverlay->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->PlayerOverlay->InventoryWidget->UpdateAllSlots();
			PlayerHUD->AlchemyOverlay->CharacterInventory->UpdateAllSlots();
			FInputModeGameOnly InputModeGameOnly;
			SetInputMode(InputModeGameOnly);
			SetShowMouseCursor(false);
			CurrentCharacter->bIsDoingAlchemy = false;
			ClearAlchemySelection();
			
		}
	}
}

void AMyPlayerController::ToggleDialogueOverlay()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleDialogueOverlay"))
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetCharacter()) : CurrentCharacter;
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(!CurrentCharacter) UE_LOG(LogTemp, Warning, TEXT("CurrentCharacter NULL"))
	
	if(!PlayerHUD) UE_LOG(LogTemp, Warning, TEXT("PlayerHUD NULL"))
	else if(!PlayerHUD->DialogueOverlay) UE_LOG(LogTemp, Warning, TEXT("DialogueOverlay NULL"))
	
	if(PlayerHUD && PlayerHUD->DialogueOverlay && PlayerHUD->DialogueOverlay->DialogueManager && CurrentCharacter && PlayerHUD->PlayerOverlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("DialogueOverlay ok"))
		if(PlayerHUD->DialogueOverlay->GetVisibility() == ESlateVisibility::Collapsed)
		{
			PlayerHUD->DialogueOverlay->SetVisibility(ESlateVisibility::Visible);
			HideOtherOverlays(2);
			FInputModeUIOnly InputModeUIOnly;
			SetInputMode(InputModeUIOnly);
			SetShowMouseCursor(true);
			CurrentCharacter->bIsConversing = true;
			SetIgnoreMoveInput(true);
			PlayerHUD->DialogueOverlay->DialogueManager->StartDialogue(CurrentCharacter->GetCurrentNPC_ID());
			//PlayerHUD->DialogueOverlay->DialogueBox->AddToListView(PlayerHUD->DialogueOverlay->DialogueManager->GetOptionStrings());
			PlayerHUD->DialogueOverlay->DialogueBox->AddToListView(PlayerHUD->DialogueOverlay->DialogueManager->GetCurrentDialogueOptions());
			PlayerHUD->DialogueOverlay->MultiLineTextBox->SetText(FText::FromString(PlayerHUD->DialogueOverlay->DialogueManager->GetNPCDialogue()));
		}
		else
		{
			PlayerHUD->DialogueOverlay->SetVisibility(ESlateVisibility::Collapsed);
			PlayerHUD->PlayerOverlay->SetVisibility(ESlateVisibility::Visible);
			FInputModeGameOnly InputModeGameOnly;
			SetInputMode(InputModeGameOnly);
			SetShowMouseCursor(false);
			CurrentCharacter->bIsConversing = false;
			CurrentCharacter->SetCurrentNPC(nullptr);
			SetIgnoreMoveInput(false);
			PlayerHUD->DialogueOverlay->DialogueBox->EmptyListView();
			PlayerHUD->DialogueOverlay->MultiLineTextBox->SetText(FText::GetEmpty());
		}
	}
}

void AMyPlayerController::HideOtherOverlays(const uint8 OverlayIndex)
{
	if(!PlayerHUD || !PlayerHUD->PlayerOverlay || !PlayerHUD->AlchemyOverlay || !PlayerHUD->DialogueOverlay) return;
	
	if(OverlayIndex == 0)
	{
		PlayerHUD->AlchemyOverlay->SetVisibility(ESlateVisibility::Collapsed);
		PlayerHUD->DialogueOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}else if(OverlayIndex == 1)
	{
		PlayerHUD->PlayerOverlay->SetVisibility(ESlateVisibility::Collapsed);
		PlayerHUD->DialogueOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}else if(OverlayIndex == 2)
	{
		PlayerHUD->PlayerOverlay->SetVisibility(ESlateVisibility::Collapsed);
		PlayerHUD->AlchemyOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyPlayerController::SelectAlchemyIngredient(const int32 SelectedSlot)
{
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetCharacter()) : CurrentCharacter;
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(!PlayerHUD || !PlayerHUD->AlchemyOverlay || !PlayerHUD->AlchemyOverlay->CharacterInventory || !PlayerHUD->PlayerOverlay || !CurrentCharacter) return;

#define INVENTORY(x) CurrentCharacter->GetInventoryComponent()->GetInventory()[x]
	
	if(INVENTORY(SelectedSlot).ItemClass->ImplementsInterface(UIngredient::StaticClass())) //If item is an ingredient
	{
		//UE_LOG(LogTemp, Warning, TEXT("Primary Substance: %s"), *UEnum::GetDisplayValueAsText(INVENTORY(SelectedSlot).IngredientInfo.PrimarySubstance).ToString());
		if(INVENTORY(SelectedSlot).IngredientInfo.IngredientType == EIngredientType::EIT_Substance)
		{
			for(int i = 0; i < 4; i++)
			{
				UInventorySlot* TempSlot = Cast<UInventorySlot>(PlayerHUD->AlchemyOverlay->TableInventory->GetInventoryGrid()->GetChildAt(i));
				if(TempSlot && TempSlot->bEmpty && !SelectedSubstances.Contains(INVENTORY(SelectedSlot).IngredientInfo.PrimarySubstance)) //No duplicate primary substances allowed
				{
					TempSlot->InventoryIndex = SelectedSlot;
					TempSlot->SlotIcon->SetBrushFromTexture(INVENTORY(SelectedSlot).ItemIcon);
					TempSlot->SlotIcon->SetOpacity(AlchemyItemIconOpacity);
					TempSlot->bEmpty = false;
					TempSlot->BackgroundImage->SetOpacity(AlchemyItemBGOpacity);

					if(CurrentRecipeMap.Contains(INVENTORY(SelectedSlot).IngredientInfo.PrimarySubstance))
					{
						//For showing how much of the currently selected ingredient is needed
						const FString AmountString =
							FString::Printf(TEXT("/%d"), CurrentRecipeMap[INVENTORY(SelectedSlot).IngredientInfo.PrimarySubstance]
							* QuantityValue::GetQuantityValueInt(INVENTORY(SelectedSlot).IngredientInfo.IngredientQuantityValue));
					
						TempSlot->AmountText->SetText(FText::FromString(AmountString));
					}
					SelectedSubstances.AddUnique(INVENTORY(SelectedSlot).IngredientInfo.PrimarySubstance);
					IngredientInfos.AddUnique(INVENTORY(SelectedSlot).IngredientInfo);
					PlayerHUD->AlchemyOverlay->UpdateInfoBox(IngredientInfos);
					break;
				}
			}
		}
	}
}

void AMyPlayerController::ClearAlchemySelection(const int32 Index)
{
	if(!PlayerHUD || !PlayerHUD->AlchemyOverlay || !PlayerHUD->AlchemyOverlay->TableInventory) return;
	
	if(Index == -1)
	{
		for(int i = 0; i < 9; i++)
		{
			UInventorySlot* TempSlot = Cast<UInventorySlot>(PlayerHUD->AlchemyOverlay->TableInventory->GetInventoryGrid()->GetChildAt(i));
			if(TempSlot)
			{
				TempSlot->InventoryIndex = -1;
				TempSlot->bEmpty = true;
				TempSlot->SlotIcon->SetBrush(FSlateBrush());
				TempSlot->SlotIcon->SetOpacity(0.f);
				TempSlot->BackgroundImage->SetOpacity(0.25f);
				TempSlot->AmountText->SetText(FText::FromString("/0"));
			}
		}
		SelectedSubstances.Empty();
		IngredientInfos.Empty();
		PlayerHUD->AlchemyOverlay->UpdateInfoBox(IngredientInfos);
	}
	else
	{
		UInventorySlot* TempSlot = Cast<UInventorySlot>(PlayerHUD->AlchemyOverlay->TableInventory->GetInventoryGrid()->GetChildAt(Index));
		if(TempSlot)
		{
			if(CurrentCharacter->GetInventoryComponent()->GetInventory().Num() > TempSlot->InventoryIndex && TempSlot->InventoryIndex >= 0)
			{
				SelectedSubstances.Remove(INVENTORY(TempSlot->InventoryIndex).IngredientInfo.PrimarySubstance);
				IngredientInfos.Remove(INVENTORY(TempSlot->InventoryIndex).IngredientInfo);
				PlayerHUD->AlchemyOverlay->UpdateInfoBox(IngredientInfos);
			}
			TempSlot->InventoryIndex = -1;
			TempSlot->bEmpty = true;
			TempSlot->SlotIcon->SetBrush(FSlateBrush());
			TempSlot->SlotIcon->SetOpacity(0.f);
			TempSlot->BackgroundImage->SetOpacity(0.25f);
		}
	}
}

void AMyPlayerController::FindIngredients(const FName& RecipeName)
{
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetCharacter()) : CurrentCharacter;
	if(CurrentCharacter == nullptr) return;

	ClearAlchemySelection();
	
	FString RecipeDataTablePath(TEXT("DataTable'/Game/Assets/Datatables/RecipeDataTable.RecipeDataTable'")); //TODO: Make this global
	UDataTable* RecipeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RecipeDataTablePath)); //TODO: Make this global as well
	if(!RecipeTableObject) return;
	
	FRecipeTable* RecipeDataRow = nullptr;
	RecipeDataRow = RecipeTableObject->FindRow<FRecipeTable>(RecipeName, TEXT(""));
	if(RecipeDataRow)
	{
		CurrentRecipeMap = RecipeDataRow->AmountPerSubstanceMap;
		for(const auto& ItemSlot : CurrentCharacter->GetInventoryComponent()->GetInventory())
		{
			for(const auto& Subs : RecipeDataRow->AmountPerSubstanceMap)
			{
				if(Subs.Key == ItemSlot.IngredientInfo.PrimarySubstance)
				{
					SelectAlchemyIngredient(ItemSlot.SlotId);
				}
			}
		}
	}
}

void AMyPlayerController::PlaySound(const FName& SFXName)
{
	const FString SFXDataTablePath(TEXT("DataTable'/Game/Assets/Datatables/SoundFXDataTable.SoundFXDataTable'")); //TODO: Make this global
	const UDataTable* SFXTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *SFXDataTablePath)); //TODO: Make this global as well
	if(!SFXTableObject) return;
	
	const FSoundEffectTable* SFXDataRow = nullptr;
	SFXDataRow = SFXTableObject->FindRow<FSoundEffectTable>(SFXName, TEXT(""));
	
	if(SFXDataRow && SFXDataRow->SFX) UGameplayStatics::PlaySound2D(this, SFXDataRow->SFX);
	
}

void AMyPlayerController::CreateNoiseNative(const float InVolume, const float InMaxRange, TOptional<FName> InTag)
{
	if(GetPawn() == nullptr || PawnNoiseEmitterComponent == nullptr) return;
	PawnNoiseEmitterComponent->MakeNoise(GetPawn(), InVolume, GetPawn()->GetActorLocation());
	//if(InTag.IsSet()) MakeNoise(InVolume, GetPawn(), GetPawn()->GetActorLocation(), InMaxRange, InTag.GetValue());
	//else MakeNoise(InVolume, GetPawn(), GetPawn()->GetActorLocation(), InMaxRange);
	
}

void AMyPlayerController::CreateNoise(const float InVolume, const float InMaxRange, FName InTag)
{
	if(GetPawn() == nullptr || PawnNoiseEmitterComponent == nullptr) return;
	//MakeNoise(InVolume, GetPawn(), GetPawn()->GetActorLocation(), InMaxRange, InTag);
	PawnNoiseEmitterComponent->MakeNoise(GetPawn(), InVolume, GetPawn()->GetActorLocation());
}

void AMyPlayerController::OnClicked_Delegate()
{
	ToggleDialogueOverlay();
}


FGenericTeamId AMyPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD)
	{
		PlayerHUD->AddCharacterOverlay();
		if(PlayerHUD->DialogueOverlay && PlayerHUD->DialogueOverlay->ExitButton && PlayerHUD->DialogueOverlay->DialogueManager)
		{
			PlayerHUD->DialogueOverlay->ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnClicked_Delegate);
			PlayerHUD->DialogueOverlay->Player = Cast<APlayerCharacter>(GetPawn());
			PlayerHUD->DialogueOverlay->DialogueManager->OnDialogueEnd.AddDynamic(this, &AMyPlayerController::OnEndDialogue);
		}
	}
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("OnPossess"))
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(!PlayerHUD) {UE_LOG(LogTemp, Warning, TEXT("PlayerHUD not valid"))}
	else if(!PlayerHUD->DialogueOverlay) {UE_LOG(LogTemp, Warning, TEXT("DialogueOverlay not valid"))}
	else if(!PlayerHUD->DialogueOverlay) {UE_LOG(LogTemp, Warning, TEXT("DialogueManager not valid"))}
	else if(PlayerHUD && PlayerHUD->DialogueOverlay && PlayerHUD->DialogueOverlay->DialogueManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Was valid"))
		PlayerHUD->DialogueOverlay->DialogueManager->OnDialogueEnd.AddDynamic(this, &AMyPlayerController::OnEndDialogue);
	}
	*/
	
	Super::OnPossess(InPawn);
}

void AMyPlayerController::OnEndDialogue()
{
	UE_LOG(LogTemp, Warning, TEXT("OnEndDialogue"))
	ToggleDialogueOverlay();
}


