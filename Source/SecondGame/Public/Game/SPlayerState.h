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

	// �Ӽ��� �ʱ�ȭ �Լ�
	void InitPlayerState();

	// 'MaxKillCount' Getter, Setter �Լ�
	int32 GetMaxKillCount() const { return MaxKillCount; }
	void SetMaxKillCount(int32 InMaxKillCount) { MaxKillCount = InMaxKillCount; }
	// 'CurrentKillCount' Getter, Setter �Լ�
	int32 GetCurrentKillCount() const { return CurrentKillCount; }
	void AddCurrentKillCount(int32 InCurrentKillCount);

	// 'MaxDeathCount' Getter, Setter �Լ�
	int32 GetMaxDeathCount() const { return MaxDeathCount; }
	void SetMaxDeathCount(int32 InMaxDeathCount) { MaxDeathCount = InMaxDeathCount; }
	// 'CurrentDeathCount' Getter, Setter �Լ�
	int32 GetCurrentDeathCount() const { return CurrentDeathCount; }
	void AddCurrentDeathCount(int32 InCurrentDeathCount);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	// 'CurrentKillCount' ��ȭ ������Ʈ
	UFUNCTION(NetMulticast, Reliable)
	void OnCurrentKillCountChanged_NetMulticast(int32 InCurrentKillCount);
	// 'CurrentDeathCount' ��ȭ ������Ʈ
	UFUNCTION(NetMulticast, Reliable)
	void OnCurrentDeathCountChanged_NetMulticast(int32 InCurrentDeathCount);

public:
	// 'CurrentKillCount'�� ��ȭ�ϸ� BroadCast�ϴ� ��������Ʈ
	FOnCurrentKillCountChangeDelegate OnCurrentKillCountChangedDelegate;
	// 'CurrentDeathCount'�� ��ȭ�ϸ� BroadCast�ϴ� ��������Ʈ
	FOnCurrentDeathCountChangeDelegate OnCurrentDeathCountChangedDelegate;

	// 'CurrentKillCount'�� 'MaxKillCount'�� �����ϸ� BroadCast�ϴ� ��������Ʈ
	FOnCurrentKillCountReachMaxDelegate OnCurrentKillCountReachedMaxDelegate;
	// 'CurrentDeathCount'�� 'MaxDeathCount'�� �����ϸ� BroadCast�ϴ� ��������Ʈ
	FOnCurrentDeathCountReachMaxDelegate OnCurrentDeathCountReachedMaxDelegate;

private:
	// �÷��̾� ĳ������ ���� ų ��
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentKillCount = 0;
	// �÷��̾� ĳ������ �ִ� ų ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 MaxKillCount = 5;

	// �÷��̾� ĳ������ ���� ���� ��
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentDeathCount = 0;
	// �÷��̾� ĳ������ �ִ� ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 MaxDeathCount = 3;
};
