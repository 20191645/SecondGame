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
	// �÷��̾� ���� ����("PlayerInfo.txt") ��������
	const FString SavedDirectoryPath = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	const FString SavedFileName(TEXT("PlayerInfo.txt"));
	FString AbsoluteFilePath = FPaths::Combine(*SavedDirectoryPath, *SavedFileName);
	FPaths::MakeStandardFilename(AbsoluteFilePath);

	// �÷��̾� ���� ����("PlayerInfo.txt")���� Json ������Ʈ ��������
	FString PlayerInfoJsonString;
	FFileHelper::LoadFileToString(PlayerInfoJsonString, *AbsoluteFilePath);
	TSharedRef<TJsonReader<TCHAR>> JsonReaderArchive = TJsonReaderFactory<TCHAR>::Create(PlayerInfoJsonString);

	// Json ������Ʈ�� ������ȭ�Ͽ� �÷��̾� �̸� ��������
	TSharedPtr<FJsonObject> PlayerInfoJsonObject = nullptr;
	if (FJsonSerializer::Deserialize(JsonReaderArchive, PlayerInfoJsonObject) == true)
	{
		FString PlayerNameString = PlayerInfoJsonObject->GetStringField(TEXT("playername"));
		// �÷��̾� �̸� �ʱ�ȭ
		// -- PlayerName: 'APlayerState' Ŭ������ �⺻������ ���Ե� �Ӽ�
		SetPlayerName(PlayerNameString);
	}

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
