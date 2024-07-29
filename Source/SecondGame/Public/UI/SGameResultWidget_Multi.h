// SGameReulstWidget_Multi.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameResultWidget_Multi.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class SECONDGAME_API USGameResultWidget_Multi : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	// 'ReturnButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnReturnButtonClicked();
	// 'ExitButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnExitButtonClicked();
	
public:
	// 플레이어 게임 순위
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> RankingText;

	// 타이틀 화면 돌아가기 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> ReturnButton;
	// 게임 종료 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;
};
