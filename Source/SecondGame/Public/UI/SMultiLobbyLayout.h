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
	// 'SubmitButton' Ŭ�� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnSubmitButtonClicked();

protected:
	// �÷��̾� �̸� �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USMultiLobbyLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UEditableTextBox> EditPlayerName;
	// �÷��̾� �̸� ���� ��ư ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USMultiLobbyLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> SubmitButton;

	// ���� IP �Է� �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USMultiLobbyLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UEditableTextBox> EditServerIP;
};
