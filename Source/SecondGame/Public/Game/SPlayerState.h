// SPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentKillCountChangeDelegate, int32, InOldCurrentKillCount, int32, InNewCurrentKillCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentDeathCountChangeDelegate, int32, InOldCurrentDeathCount, int32, InNewCurrentDeathCount);

UCLASS()
class SECONDGAME_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASPlayerState();

	// 속성값 초기화 함수
	void InitPlayerState();

	// 'MaxKillCount' Getter, Setter 함수
	int32 GetMaxKillCount() const { return MaxKillCount; }
	void SetMaxKillCount(int32 InMaxKillCount) { MaxKillCount = InMaxKillCount; }
	// 'CurrentKillCount' Getter, Setter 함수
	int32 GetCurrentKillCount() const { return CurrentKillCount; }
	void AddCurrentKillCount(int32 InCurrentKillCount);

	// 'MaxDeathCount' Getter, Setter 함수
	int32 GetMaxDeathCount() const { return MaxDeathCount; }
	void SetMaxDeathCount(int32 InMaxDeathCount) { MaxDeathCount = InMaxDeathCount; }
	// 'CurrentDeathCount' Getter, Setter 함수
	int32 GetCurrentDeathCount() const { return CurrentDeathCount; }
	void AddCurrentDeathCount(int32 InCurrentDeathCount);

public:
	// 'CurrentKillCount'가 변화하면 BroadCast하는 델리게이트
	FOnCurrentKillCountChangeDelegate OnCurrentKillCountChangedDelegate;

	// 'CurrentDeathCount'가 변화하면 BroadCast하는 델리게이트
	FOnCurrentDeathCountChangeDelegate OnCurrentDeathCountChangedDelegate;

private:
	// 플레이어 캐릭터의 현재 킬 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentKillCount = 0;
	// 플레이어 캐릭터의 최대 킬 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 MaxKillCount = 10;

	// 플레이어 캐릭터의 현재 데스 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentDeathCount = 0;
	// 플레이어 캐릭터의 최대 데스 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 MaxDeathCount = 3;
};
