// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueBox.h"

void UDialogueBox::SetSelectedElement(const int32 ElementIndex)
{
	if(ElementArray.IsValidIndex(ElementIndex))
	{
		SelectedElement = ElementArray[ElementIndex];
	}
}
