// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/CustomAIContainer.h"

void ReverseArray(TArray<AActor*>& InArray, int32 Low, int32 High)
{
	while(Low < High)
	{
		std::swap(InArray[Low], InArray[High]);
		Low++;
		High--;
	}
}
//K is the number of spots each element is shifted
void ShiftLeftByN(TArray<AActor*>& InArray, int32 N, int32 K)
{
	K = K%N;
	ReverseArray(InArray, 0, K-1);
	ReverseArray(InArray, K, N-1);
	ReverseArray(InArray, 0, N-1);
}


void UCustomAIContainer::LeftShiftContainer()
{
	if(ActorContainer.IsEmpty() || ActorContainer.Num() < 2) return;
	constexpr int K = 1;
	ShiftLeftByN(ActorContainer, ActorContainer.Num(), K);
}
