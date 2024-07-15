// SMultiLobbyLayout.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SMultiLobbyLayout.generated.h"

class UButton;
class UEditableTextBox;

UCLASS()
class SECONDGAME_API USMultiLobbyLayout : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

private:
	// 'SubmitButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnSubmitButtonClicked();

protected:
	// 플레이어 이름 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USMultiLobbyLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UEditableTextBox> EditPlayerName;
	// 플레이어 이름 제출 버튼 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USMultiLobbyLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> SubmitButton;

	// 서버 IP 입력 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USMultiLobbyLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UEditableTextBox> EditServerIP;
};
