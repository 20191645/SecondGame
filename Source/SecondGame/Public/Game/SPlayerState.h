// SPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentKillCountChangeDelegate, int32, InOldCurrentKillCount, int32, InNewCurrentKillCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentDeathCountChangeDelegate, int32, InOldCurrentDeathCount, int32, InNewCurrentDeathCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentKillCountReachMaxDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentDeathCountReachMaxDelegate);

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	// 'CurrentKillCount' 변화 업데이트
	UFUNCTION(NetMulticast, Reliable)
	void OnCurrentKillCountChanged_NetMulticast(int32 InCurrentKillCount);
	// 'CurrentDeathCount' 변화 업데이트
	UFUNCTION(NetMulticast, Reliable)
	void OnCurrentDeathCountChanged_NetMulticast(int32 InCurrentDeathCount);

public:
	// 'CurrentKillCount'가 변화하면 BroadCast하는 델리게이트
	FOnCurrentKillCountChangeDelegate OnCurrentKillCountChangedDelegate;
	// 'CurrentDeathCount'가 변화하면 BroadCast하는 델리게이트
	FOnCurrentDeathCountChangeDelegate OnCurrentDeathCountChangedDelegate;

	// 'CurrentKillCount'가 'MaxKillCount'에 도달하면 BroadCast하는 델리게이트
	FOnCurrentKillCountReachMaxDelegate OnCurrentKillCountReachedMaxDelegate;
	// 'CurrentDeathCount'가 'MaxDeathCount'에 도달하면 BroadCast하는 델리게이트
	FOnCurrentDeathCountReachMaxDelegate OnCurrentDeathCountReachedMaxDelegate;

private:
	// 플레이어 캐릭터의 현재 킬 수
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentKillCount = 0;
	// 플레이어 캐릭터의 최대 킬 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 MaxKillCount = 5;

	// 플레이어 캐릭터의 현재 데스 수
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentDeathCount = 0;
	// 플레이어 캐릭터의 최대 데스 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 MaxDeathCount = 3;
};
