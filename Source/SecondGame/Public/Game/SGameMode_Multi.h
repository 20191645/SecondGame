// SGameMode_Multi.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode_Multi.generated.h"

class ASPlayerController_Multi;

UCLASS()
class SECONDGAME_API ASGameMode_Multi : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameMode_Multi();

	// �÷��̾ ���ӿ� ������ �� ȣ��� �Լ�
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// �÷��̾ ���ӿ��� ���� �� ȣ��� �Լ�
	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

	// �÷��̾� Ż�� �� ȣ��
	void OnControllerDead(ASPlayerController_Multi* InDeadController);

	// 'AlivePlayerControllers' Getter �Լ�
	TArray<ASPlayerController_Multi*> GetAlivePlayerControllers() { return AlivePlayerControllers; }

private:
	// 'MainTimerHandle' Ÿ�̸ӿ� ������ �Լ�
	UFUNCTION()
	void OnMainTimerElapsed();

	// ���ӿ� ������ ��� �÷��̾�� �˸� �޽��� ����
	void NotifyToAllPlayer(const FString& NotificationString);

public:
	FTimerHandle MainTimerHandle;

	// �⺻ ���� ���� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi")
	int32 WaitingTime = 7;
	// ������ ���ӿ� �����ϱ���� ���� �ð�
	int32 RemainWaitingTimeForPlaying = 7;
	// ���� ���� �ּ� �ο�
	int32 MinimumPlayerCountForPlaying = 2;

	// �⺻ ���� ���� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi")
	int32 EndingTime = 10;
	// ���� ������� ���� �ð�
	int32 RemainWaitingTimeForEnding = 10;

protected:
	// ���ӿ� �����ִ� �÷��̾�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController_Multi>> AlivePlayerControllers;
	// ���ӿ��� Ż���� �÷��̾�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController_Multi>> DeadPlayerControllers;
};
