#include "MyTriggerBox.h"
#include "../Public/MyCharacter.h"
#include "../danganronpaProjectile.h"  // �����ӵ���ͷ�ļ�
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
//Refactored with adaptor Pattern
// ���캯��
AMyTriggerBox::AMyTriggerBox()
{
    // ��ʼ��������
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    // ���ô�������ײ����
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
}

// ���ص�����ʱ�Ļص�����
void AMyTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(AdanganronpaProjectile::StaticClass())) // �ж��Ƿ�Ϊ�ӵ�
    {
        UE_LOG(LogTemp, Warning, TEXT("�ӵ��봥��������ײ"));

        // ִ����Ӧ�Ĵ����¼�
        OnTriggerHit();  // ִ�д������¼�
    }
}

// ���ݴ�����ִ�в�ͬ���߼�
void AMyTriggerBox::OnTriggerHit()
{
    // ����Ը��ݴ���������ƻ���������ִ�в�ͬ���߼�
    if (this->GetName() == TEXT("triggerFalse"))
    {
        UE_LOG(LogTemp, Warning, TEXT("False ������"));
        TriggerDeath();
        // ������1���߼�
    }
    else if (this->GetName() == TEXT("triggerRight"))
    {
        // ִ�д���һ���ؿ��Ĳ���
        UGameplayStatics::OpenLevel(this, FName("dialogueEndStage"));

        UE_LOG(LogTemp, Warning, TEXT("Right ������"));
        // ������2���߼�
    }
}

void AMyTriggerBox::TriggerDeath()
{
    // ��ȡ��ǰ��ҿ�����
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        // ��ȡ��ǰ��ҿ��ƵĽ�ɫ
        AMyCharacter* MyCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());

        // ����ɹ�ת��Ϊ MyCharacter����������������
        if (MyCharacter)
        {
            MyCharacter->Death(); // ������������
        }
    }
}

void AMyTriggerBox::OpenNextLevel()
{
    // ���� Dialogue2 �ؿ�
    UGameplayStatics::OpenLevel(this, TEXT("Dialogue2"));
}


