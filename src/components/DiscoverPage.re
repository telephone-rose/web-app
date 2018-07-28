type file = {id: string};

type downloadableFile = {
  id: string,
  downloadUrl: string,
};

type location = {
  latitude: float,
  longitude: float,
};

type messageFile = {
  id: string,
  emojiResume: option(string),
  compressedFile: downloadableFile,
};

type user = {
  id: string,
  firstName: string,
  distance: option(int),
  city: option(string),
  country: option(string),
  answeringMessageFile: option(messageFile),
};

type discover = {
  firstName: string,
  answeringMessageFile: option(file),
  location: option(location),
  randomUserFeed: array(user),
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
        randomUserFeed (
          distance: 999999999,
          pagination: {
            limit: 10
            offset: 0
          }
        ) @bsRecord {
          id
          firstName
          distance
          country
          city
          answeringMessageFile @bsRecord {
            id
            emojiResume(length: 3)
            compressedFile @bsRecord {
              id
              downloadUrl
            }
          }
        }
      }
    }
  |}
];

module Query = ReasonApollo.CreateQuery(Payload);

let component = ReasonReact.statelessComponent("DiscoverPage");

let make = _children => {
  ...component,
  render: _self =>
    <Query>
      ...(
           ({result}) =>
             switch (result) {
             | Loading => <div> (ReasonReact.string("Loading")) </div>
             | Error(error) =>
               <div> (ReasonReact.string(error##message)) </div>
             | Data(response) =>
               <div>
                 {
                   let me = response##me;
                   me.randomUserFeed
                   |> Array.map(randomUser =>
                        <div key=randomUser.id className=Theme.marginTop>
                          <DiscoverCard
                            emojiResume=(
                              switch (randomUser.answeringMessageFile) {
                              | None => None
                              | Some(answeringMessageFile) =>
                                answeringMessageFile.emojiResume
                              }
                            )
                            recordingDownloadUrl=(
                              switch (randomUser.answeringMessageFile) {
                              | None => None
                              | Some(answeringMessageFile) =>
                                Some(
                                  answeringMessageFile.compressedFile.
                                    downloadUrl,
                                )
                              }
                            )
                            userCity=randomUser.city
                            distance=randomUser.distance
                            userId=randomUser.id
                            userFirstName=randomUser.firstName
                          />
                        </div>
                      )
                   |> ReasonReact.array;
                 }
               </div>
             }
         )
    </Query>,
};
