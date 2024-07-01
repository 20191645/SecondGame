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

public:
	// 'CurrentKillCount'�� ��ȭ�ϸ� BroadCast�ϴ� ��������Ʈ
	FOnCurrentKillCountChangeDelegate OnCurrentKillCountChangedDelegate;

	// 'CurrentDeathCount'�� ��ȭ�ϸ� BroadCast�ϴ� ��������Ʈ
	FOnCurrentDeathCountChangeDelegate OnCurrentDeathCountChangedDelegate;

private:
	// �÷��̾� ĳ������ ���� ų ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentKillCount = 0;
	// �÷��̾� ĳ������ �ִ� ų ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 MaxKillCount = 10;

	// �÷��̾� ĳ������ ���� ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentDeathCount = 0;
	// �÷��̾� ĳ������ �ִ� ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 MaxDeathCount = 3;
};
