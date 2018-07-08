type updateLocation = {id: string};

module Payload = [%graphql
  {|
    mutation UpdateLocation (
      $location: GeometryPointInput!
    ) {
      updateLocation: sendSelfLocation (
        location: $location
      ) @bsRecord  {
        id
      }
    }
  |}
];

module Mutation = ReasonApollo.CreateMutation(Payload);
