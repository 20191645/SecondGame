// SGameMode_Multi.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode_Multi.generated.h"

UCLASS()
class SECONDGAME_API ASGameMode_Multi : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameMode_Multi();

	// �÷��̾ ���ӿ� ������ �� ȣ��� �Լ�
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
