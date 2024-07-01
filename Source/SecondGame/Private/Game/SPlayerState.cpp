// SPlayerState.cpp

#include "Game/SPlayerState.h"
#include "Character/SPlayerCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ASPlayerState::ASPlayerState()
{
}

void ASPlayerState::InitPlayerState()
{
	// 플레이어 이름 초기화
	// -- PlayerName: 'APlayerState' 클래스에 기본적으로 포함된 속성
	SetPlayerName(TEXT("Player"));

	// 모든 속성값 초기화
	CurrentKillCount = 0;
	MaxKillCount = 10;
	CurrentDeathCount = 0;
	MaxDeathCount = 3;
}

void ASPlayerState::AddCurrentKillCount(int32 InCurrentKillCount)
{
	// 'CurrentKillCountChangedDelegate'에 바인드된 함수에게 Broadcast
	OnCurrentKillCountChangedDelegate.Broadcast(CurrentKillCount, CurrentKillCount + InCurrentKillCount);

	CurrentKillCount = FMath::Clamp(CurrentKillCount + InCurrentKillCount, 0, MaxKillCount);
}

void ASPlayerState::AddCurrentDeathCount(int32 InCurrentDeathCount)
{
	// 'CurrentDeathCountChangedDelegate'에 바인드된 함수에게 Broadcast
	OnCurrentDeathCountChangedDelegate.Broadcast(CurrentDeathCount, CurrentDeathCount + InCurrentDeathCount);

	CurrentDeathCount = FMath::Clamp(CurrentDeathCount + InCurrentDeathCount, 0, MaxDeathCount);
}
