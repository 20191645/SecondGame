// SGameResultWidget_Single.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameResultWidget_Single.generated.h"

class UButton;

UCLASS()
class SECONDGAME_API USGameResultWidget_Single : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USGameResultWidget_Single(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

	// 'ReturnButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnReturnButtonClicked();
	// 'ExitButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnExitButtonClicked();

private:
	// 타이틀 화면 돌아가기 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> ReturnButton;
	// 게임 종료 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> ExitButton;
};
