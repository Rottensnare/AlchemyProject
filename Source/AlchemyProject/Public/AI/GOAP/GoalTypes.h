#pragma once

UENUM()
enum class EGoalType : uint8
{
	EGT_MAX,
	EGT_Idle,
	EGT_Goto,
	EGT_TakeCover,
	EGT_FollowOther,
	EGT_Defend,
	EGT_Guard,
	EGT_Patrol,
	EGT_Search,
	EGT_Interact
};