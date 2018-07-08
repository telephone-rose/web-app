type updateAnsweringMessage = {id: string};

module UpdateAnsweringMessage = [%graphql
  {|
    mutation UpdateAnsweringMessage (
      $recordingId: ID!
    ) {
      updateAnsweringMessage (
        recordingId: $recordingId
      ) @bsRecord {
        id
      }
    }
  |}
];

module Mutation = ReasonApollo.CreateMutation(UpdateAnsweringMessage);
