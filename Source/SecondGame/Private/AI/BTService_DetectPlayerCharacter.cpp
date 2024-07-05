// BTService_DetectPlayerCharacter.cpp

#include "AI/BTService_DetectPlayerCharacter.h"
#include "Controller/SAIController.h"
#include "Character/SNonPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/SPlayerCharacter.h"

UBTService_DetectPlayerCharacter::UBTService_DetectPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Behavior Tree�� ���� ��� �̸� ����
	NodeName = TEXT("DetectPlayerCharacter");
	// TickNode() �Լ� ȣ�� �ֱ� ����
	Interval = 1.f;
}

void UBTService_DetectPlayerCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASAIController* AIC = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIC) == true)
	{
		ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIC->GetPawn());
		if (IsValid(NPC) == true)
		{
			UWorld* World = NPC->GetWorld();
			if (IsValid(World) == true)
			{
				FVector CenterPosition = NPC->GetActorLocation();
				// �÷��̾� ĳ���� Ž�� �Ÿ�
				float DetectRadius = 1500.f;

				// �ϳ� �̻��� ������Ʈ�� �����ðű� ������ TArray ���
				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams CollisionQueryParams(NAME_None, false, NPC);
				// ���� ������Ʈ�� ���� -- {Overlap}{Multi}{ByChannel}
				bool bResult = World->OverlapMultiByChannel(
					OverlapResults,
					CenterPosition,
					FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel3,	// = 'Detect' Trace Channel
					FCollisionShape::MakeSphere(DetectRadius),
					CollisionQueryParams
				);

				// ������Ʈ�� ������ ���, Player Character ��������
				if (bResult == true)
				{
					for (auto const& OverlapResult : OverlapResults)
					{
						ASPlayerCharacter* PC = Cast<ASPlayerCharacter>(OverlapResult.GetActor());
						// IsPlayerController(): ������ ĳ���Ͱ� �÷��̾ �����ϰ� �ִ� ������ Ȯ��
						if (IsValid(PC) == true && PC->GetController()->IsPlayerController() == true)
						{
							// �÷��̾� ĳ���� Ž�� ����
							// 'BB_NPC'�� 'TargetActor' Key ���� 'PlayerCharacter'�� ����
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, PC);
						}
					}
				}
				else
				{
					// 'BB_NPC'�� 'TargetActor' Key ���� nullptr�� ����
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, nullptr);
				}
			}
		}
	}
}
