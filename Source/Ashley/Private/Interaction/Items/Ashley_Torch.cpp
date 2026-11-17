// Ashley


#include "Interaction/Items/Ashley_Torch.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"

#include "Core/Ashley_TriggerCollider.h"

#include "DebugHelper.h"
#include <Kismet/GameplayStatics.h>
#include <Managers/Ashley_EventManagerSystem.h>


AAshley_Torch::AAshley_Torch()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBase"));
    MeshBase->SetupAttachment(RootComponent);

    Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotlightComponent"));
    Light->SetupAttachment(RootComponent);

    LightPoint = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointlightComponent"));
    LightPoint->SetupAttachment(RootComponent);

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);

    // Configuro il layer per essere interagibile dal player
    MeshBase->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

    MeshBase->SetRenderCustomDepth(true);
    MeshBase->SetCustomDepthStencilValue(0);
}

void AAshley_Torch::BeginPlay()
{
    Super::BeginPlay();

    // TODO: spostare questa gestione specifica nel farmer
    auto CharacterParent = Cast<AAshley_CharacterPlayer>(GetParentActor());

    // Se sono gi� dentro il BP del character, mi nascondo fino a quando non viene accesa
    if (CharacterParent)
    {
        Character = CharacterParent;
        Character->SetTorch(this);
        MeshBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        BoxInteract->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        MeshBase->SetHiddenInGame(true);
        TurnOff();

        bIsEquipped = true;
    }
}

void AAshley_Torch::TurnOn()
{
    AudioComponent->Play();
    bIsOn = true;
    Light->SetVisibility(true);
    LightPoint->SetVisibility(true);
}

void AAshley_Torch::TurnOff()
{
    AudioComponent->Play();
    bIsOn = false;
    Light->SetVisibility(false);
    LightPoint->SetVisibility(false);
}

void AAshley_Torch::Interact_Implementation()
{
    Character = Cast<AAshley_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
    EquipTorch();
}

void AAshley_Torch::EquipTorch()
{
    if (Character)
    {
        Character->EquipTorchInHand(this);

        // Rimuovo le collisioni
        this->bIgnoreCollision = true;
        MeshBase->SetHiddenInGame(false);
        MeshBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        BoxInteract->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        Character->InteractAreaTriggered(false, this);

        // Inizializzo la torcia spenta?
        TurnOff();

        bIsEquipped = true;
    }
}
