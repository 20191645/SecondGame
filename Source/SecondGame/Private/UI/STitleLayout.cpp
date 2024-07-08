// STitleLayout.cpp

#include "UI/STitleLayout.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

USTitleLayout::USTitleLayout(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USTitleLayout::NativeConstruct()
{
	// ��ư Ŭ�� �̺�Ʈ�� ��� �Լ� ���ε�
	SinglePlayButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSinglePlayButtonClicked);
	MultiPlayButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnMultiPlayButtonClicked);
	ExitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void USTitleLayout::OnSinglePlayButtonClicked()
{
	// ������ �̵�
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("SingleMainLevel"));
}

void USTitleLayout::OnMultiPlayButtonClicked()
{
	// ������ �̵�
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MultiMainLevel"));
}

void USTitleLayout::OnExitButtonClicked()
{
	// ���� ����
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
