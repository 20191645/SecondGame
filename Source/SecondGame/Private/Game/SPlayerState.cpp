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
	// 플레이어 정보 파일("PlayerInfo.txt") 가져오기
	const FString SavedDirectoryPath = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	const FString SavedFileName(TEXT("PlayerInfo.txt"));
	FString AbsoluteFilePath = FPaths::Combine(*SavedDirectoryPath, *SavedFileName);
	FPaths::MakeStandardFilename(AbsoluteFilePath);

	// 플레이어 정보 파일("PlayerInfo.txt")에서 Json 오브젝트 가져오기
	FString PlayerInfoJsonString;
	FFileHelper::LoadFileToString(PlayerInfoJsonString, *AbsoluteFilePath);
	TSharedRef<TJsonReader<TCHAR>> JsonReaderArchive = TJsonReaderFactory<TCHAR>::Create(PlayerInfoJsonString);

	// Json 오브젝트를 역직렬화하여 플레이어 이름 가져오기
	TSharedPtr<FJsonObject> PlayerInfoJsonObject = nullptr;
	if (FJsonSerializer::Deserialize(JsonReaderArchive, PlayerInfoJsonObject) == true)
	{
		FString PlayerNameString = PlayerInfoJsonObject->GetStringField(TEXT("playername"));
		// 플레이어 이름 초기화
		// -- PlayerName: 'APlayerState' 클래스에 기본적으로 포함된 속성
		SetPlayerName(PlayerNameString);
	}

	// 속성값 초기화
	MaxKillCount = 5;
	MaxDeathCount = 3;
	AddCurrentKillCount(0);
	AddCurrentDeathCount(0);
}

void ASPlayerState::AddCurrentKillCount(int32 InCurrentKillCount)
{
	CurrentKillCount = FMath::Clamp(CurrentKillCount + InCurrentKillCount, 0, MaxKillCount);
	
	// 'CurrentKillCountChangedDelegate'에 바인드된 함수에게 Broadcast
	OnCurrentKillCountChangedDelegate.Broadcast(CurrentKillCount, CurrentKillCount);

	if (CurrentKillCount >= MaxKillCount) {
		OnCurrentKillCountReachedMaxDelegate.Broadcast();
	}

	// 'CurrentKillCount' 변화 업데이트
	OnCurrentKillCountChanged_NetMulticast(CurrentKillCount);
}

void ASPlayerState::AddCurrentDeathCount(int32 InCurrentDeathCount)
{
	CurrentDeathCount = FMath::Clamp(CurrentDeathCount + InCurrentDeathCount, 0, MaxDeathCount);

	// 'CurrentDeathCountChangedDelegate'에 바인드된 함수에게 Broadcast
	OnCurrentDeathCountChangedDelegate.Broadcast(CurrentDeathCount, CurrentDeathCount + InCurrentDeathCount);

	if (CurrentDeathCount >= MaxDeathCount) {
		OnCurrentDeathCountReachedMaxDelegate.Broadcast();
	}

	// 'CurrentDeathCount' 변화 업데이트
	OnCurrentDeathCountChanged_NetMulticast(CurrentDeathCount);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 해당 속성 포함해서 복제
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

	// 서버에서 클라이언트의 'PlayerName' 값으로 초기화
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
