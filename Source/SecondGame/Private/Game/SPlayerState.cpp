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
	// �÷��̾� �̸� �ʱ�ȭ
	// -- PlayerName: 'APlayerState' Ŭ������ �⺻������ ���Ե� �Ӽ�
	SetPlayerName(TEXT("Player"));

	// ��� �Ӽ��� �ʱ�ȭ
	CurrentKillCount = 0;
	MaxKillCount = 10;
	CurrentDeathCount = 0;
	MaxDeathCount = 3;
}

void ASPlayerState::AddCurrentKillCount(int32 InCurrentKillCount)
{
	// 'CurrentKillCountChangedDelegate'�� ���ε�� �Լ����� Broadcast
	OnCurrentKillCountChangedDelegate.Broadcast(CurrentKillCount, CurrentKillCount + InCurrentKillCount);

	CurrentKillCount = FMath::Clamp(CurrentKillCount + InCurrentKillCount, 0, MaxKillCount);
}

void ASPlayerState::AddCurrentDeathCount(int32 InCurrentDeathCount)
{
	// 'CurrentDeathCountChangedDelegate'�� ���ε�� �Լ����� Broadcast
	OnCurrentDeathCountChangedDelegate.Broadcast(CurrentDeathCount, CurrentDeathCount + InCurrentDeathCount);

	CurrentDeathCount = FMath::Clamp(CurrentDeathCount + InCurrentDeathCount, 0, MaxDeathCount);
}
