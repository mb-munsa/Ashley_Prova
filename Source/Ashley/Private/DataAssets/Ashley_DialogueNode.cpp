// Ashley


#include "DataAssets/Ashley_DialogueNode.h"

#if WITH_EDITOR
void UAshley_DialogueNode::PostInitProperties()
{
    Super::PostInitProperties();

    if (NodeID.IsValid() == false)
    {
        NodeID = FGuid::NewGuid();
    }
}

void UAshley_DialogueNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    for (int i = 0; i < DialogueChoices.Num(); i++)
    {
        DialogueChoices[i].ChoiceIndex = i;
    }
}
#endif