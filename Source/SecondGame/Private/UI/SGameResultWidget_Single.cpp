// SGameResultWidget_Single.cpp

#include "UI/SGameResultWidget_Single.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

USGameResultWidget_Single::USGameResultWidget_Single(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USGameResultWidget_Single::NativeConstruct()
{
	// ��ư Ŭ�� �̺�Ʈ�� ��� �Լ� ���ε�
	ReturnButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnReturnButtonClicked);
	ExitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void USGameResultWidget_Single::OnReturnButtonClicked()
{
	// �ε� ������ �̵�
	// -- �ε� ���������� 'NextLevel'�� �Ľ��ؼ� 'TitleLevel' ���� ����
	UGameplayStatics::OpenLevel(
		GetWorld(), FName(TEXT("LoadingLevel")), true, FString(TEXT("NextLevel=TitleLevel")));
}

void USGameResultWidget_Single::OnExitButtonClicked()
{
	// ���� ����
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
