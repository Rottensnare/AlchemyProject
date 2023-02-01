// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_LoopDynamic.h"

#include "AI/Utility/CustomAIContainer.h"
#include "BehaviorTree/BlackboardComponent.h"

//TODO: Really need an interface so that this can be made dynamic with different classes
void UBTDecorator_LoopDynamic::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{	
	if(SearchData.OwnerComp.GetBlackboardComponent())
	{
		// ReSharper disable once CppTooWideScope
		const auto TempObject = Cast<UCustomAIContainer>(SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsObject(KeySelector.SelectedKeyName));
		if(TempObject) NumLoops = TempObject->ActorContainer.Num();
	}
	
	Super::OnNodeActivation(SearchData);
}

