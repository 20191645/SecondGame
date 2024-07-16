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

protected:
	// ���� ����ִ� �÷��̾�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController_Multi>> AlivePlayerControllers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController_Multi>> DeadPlayerControllers;
};
