type file = {id: string};

type location = {
  latitude: float,
  longitude: float,
};

type discover = {
  firstName: string,
  answeringMessageFile: option(file),
  location: option(location),
};

module Payload = [%graphql
  {|
    query Discover {
      me @bsRecord {
        firstName
        answeringMessageFile @bsRecord {
          id
        }
        location @bsRecord {
          latitude
          longitude
        }
      }
    }
  |}
];

module Query = ReasonApollo.CreateQuery(Payload);
