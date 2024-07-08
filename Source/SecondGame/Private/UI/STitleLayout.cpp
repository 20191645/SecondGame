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
	// �ε� ������ �̵�
	// -- �ε� ���������� 'NextLevel'�� �Ľ��ؼ� 'SingleMainLevel' ���� ����
	// OpenLevel() �Լ��� Options �Ű�����: FString(TEXT("NextLevel=SingleMainLevel"))
	// => 'NextLevel'�� Key, 'SingleMainLevel'�� Value 
	UGameplayStatics::OpenLevel(
		GetWorld(), FName(TEXT("LoadingLevel")), true, FString(TEXT("NextLevel=SingleMainLevel")));
}

void USTitleLayout::OnMultiPlayButtonClicked()
{
	// �ε� ������ �̵�
	// -- �ε� ���������� 'NextLevel'�� �Ľ��ؼ� 'MultiMainLevel' ���� ����
	UGameplayStatics::OpenLevel(
		GetWorld(), FName(TEXT("LoadingLevel")), true, FString(TEXT("NextLevel=MultiMainLevel")));
}

void USTitleLayout::OnExitButtonClicked()
{
	// ���� ����
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
