// SPlayerState.cpp

#include "Game/SPlayerState.h"
#include "Character/SPlayerCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

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

	// �Ӽ��� �ʱ�ȭ
	MaxKillCount = 5;
	MaxDeathCount = 3;
	AddCurrentKillCount(0);
	AddCurrentDeathCount(0);
}

void ASPlayerState::AddCurrentKillCount(int32 InCurrentKillCount)
{
	CurrentKillCount = FMath::Clamp(CurrentKillCount + InCurrentKillCount, 0, MaxKillCount);
	
	// 'CurrentKillCountChangedDelegate'�� ���ε�� �Լ����� Broadcast
	OnCurrentKillCountChangedDelegate.Broadcast(CurrentKillCount, CurrentKillCount);

	if (CurrentKillCount >= MaxKillCount) {
		OnCurrentKillCountReachedMaxDelegate.Broadcast();
	}

	// 'CurrentKillCount' ��ȭ ������Ʈ
	OnCurrentKillCountChanged_NetMulticast(CurrentKillCount);
}

void ASPlayerState::AddCurrentDeathCount(int32 InCurrentDeathCount)
{
	CurrentDeathCount = FMath::Clamp(CurrentDeathCount + InCurrentDeathCount, 0, MaxDeathCount);

	// 'CurrentDeathCountChangedDelegate'�� ���ε�� �Լ����� Broadcast
	OnCurrentDeathCountChangedDelegate.Broadcast(CurrentDeathCount, CurrentDeathCount + InCurrentDeathCount);

	if (CurrentDeathCount >= MaxDeathCount) {
		OnCurrentDeathCountReachedMaxDelegate.Broadcast();
	}

	// 'CurrentDeathCount' ��ȭ ������Ʈ
	OnCurrentDeathCountChanged_NetMulticast(CurrentDeathCount);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �ش� �Ӽ� �����ؼ� ����
	DOREPLIFETIME(ThisClass, CurrentKillCount);
	DOREPLIFETIME(ThisClass, CurrentDeathCount);
}

void ASPlayerState::InitPlayerName_Server_Implementation(const FString& PlayerName)
{
	SetPlayerName(PlayerName);
}

void ASPlayerState::InitPlayerState_Client_Implementation()
{
	InitPlayerState();

	// �������� Ŭ���̾�Ʈ�� 'PlayerName' ������ �ʱ�ȭ
	InitPlayerName_Server(GetPlayerName());
}

void ASPlayerState::OnCurrentKillCountChanged_NetMulticast_Implementation(int32 InCurrentKillCount)
{
	if (true == OnCurrentKillCountChangedDelegate.IsBound())
	{
		OnCurrentKillCountChangedDelegate.Broadcast(CurrentKillCount, InCurrentKillCount);
	}

	if (CurrentKillCount >= MaxKillCount) {
		OnCurrentKillCountReachedMaxDelegate.Broadcast();
	}
}

void ASPlayerState::OnCurrentDeathCountChanged_NetMulticast_Implementation(int32 InCurrentDeathCount)
{
	if (true == OnCurrentDeathCountChangedDelegate.IsBound())
	{
		OnCurrentDeathCountChangedDelegate.Broadcast(CurrentDeathCount, InCurrentDeathCount);
	}

	if (CurrentDeathCount >= MaxDeathCount) {
		OnCurrentDeathCountReachedMaxDelegate.Broadcast();
	}
}
