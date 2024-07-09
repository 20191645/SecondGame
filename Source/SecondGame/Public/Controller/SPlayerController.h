// SPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class USHUD;
class USGameResultWidget_Single;

UCLASS()
class SECONDGAME_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// 'HUDWidget' Getter 함수
	USHUD* GetHUDWidget() const { return HUDWidget; };

protected:
	virtual void BeginPlay() override;

	// 'OnCurrentKillCountReachedMaxDelegate' 델리게이트에 바인드할 함수
	UFUNCTION()
	void OnCurrentKillCountReachedMax();
	// 'OnCurrentDeathCountReachedMaxDelegate' 델리게이트에 바인드할 함수
	UFUNCTION()
	void OnCurrentDeathCountReachedMax();

private:
	// 조준점 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> CrosshairUIClass;

	// HUD 위젯 클래스 객체
	UPROPERTY();
	TObjectPtr<USHUD> HUDWidget;
	// HUD 위젯 클래스 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess));
	TSubclassOf<USHUD> HUDWidgetClass;

	// 게임 클리어 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<USGameResultWidget_Single> GameClearWidgetClass;
	// 게임 오버 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<USGameResultWidget_Single> GameOverWidgetClass;
};
