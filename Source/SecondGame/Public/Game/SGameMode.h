// SGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

UCLASS()
class SECONDGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameMode();

	// �÷��̾ ���ӿ� ������ �� ȣ��� �Լ�
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
