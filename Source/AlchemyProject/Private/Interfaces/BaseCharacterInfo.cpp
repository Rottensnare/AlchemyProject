// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/BaseCharacterInfo.h"

// Add default functionality here for any IBaseCharacterInfo functions that are not pure virtual.
FNPCInfo& IBaseCharacterInfo::GetNPCInfo()
{
	DummyNPCInfo = FNPCInfo();
	return DummyNPCInfo;
}

EPhysicalSurface IBaseCharacterInfo::GetFootStepSurfaceType()
{
	
	return EPhysicalSurface::SurfaceType1;
	
}
