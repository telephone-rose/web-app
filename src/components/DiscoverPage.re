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

module DiscoverQueryPayload = [%graphql
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
            limit: 4
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
            emojiResume(length: 10)
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

module DiscoverQuery = ReasonApollo.CreateQuery(DiscoverQueryPayload);

module HideUserMutationPayload = [%graphql
  {|
    mutation HideUser (
      $userId: String!
    ) {
      hideUser (userId: $userId) {
        id
      }
    }
  |}
];

module HideUserMutation =
  ReasonApollo.CreateMutation(HideUserMutationPayload);

let component = ReasonReact.statelessComponent("DiscoverPage");

let pageStyle =
  Glamor.(
    css([
      width("100%"),
      height("100%"),
      display("flex"),
      justifyContent("center"),
      alignItems("center"),
    ])
  );
let cardWrapperStyle = Glamor.(css([position("absolute")]));

let make = _children => {
  ...component,
  render: _self =>
    <DiscoverQuery>
      ...{
           ({result}) =>
             switch (result) {
             | Loading => <div> {ReasonReact.string("Loading")} </div>
             | Error(error) =>
               <div> {ReasonReact.string(error##message)} </div>
             | Data(response) =>
               <div className=pageStyle>
                 {
                   let me = response##me;
                   me.randomUserFeed
                   |> Array.mapi((i, randomUser) => {
                        let hideUser =
                          HideUserMutationPayload.make(
                            ~userId=randomUser.id,
                            (),
                          );

                        <div
                          style={
                            ReactDOMRe.Style.make(
                              ~zIndex=
                                string_of_int(
                                  (me.randomUserFeed |> Array.length) - i,
                                ),
                              ~transform=
                                "scale("
                                ++ string_of_float(
                                     1.0 -. 0.1 *. float_of_int(i),
                                   )
                                ++ ") translateY("
                                ++ string_of_int((-20) * i)
                                ++ "px)",
                              (),
                            )
                          }
                          className=cardWrapperStyle
                          key={string_of_int(i)}>
                          <HideUserMutation>
                            ...(
                                 (mutation, _) =>
                                   <DiscoverCard
                                     emojiResume={
                                       switch (randomUser.answeringMessageFile) {
                                       | None => None
                                       | Some(answeringMessageFile) =>
                                         answeringMessageFile.emojiResume
                                       }
                                     }
                                     recordingDownloadUrl={
                                       switch (randomUser.answeringMessageFile) {
                                       | None => None
                                       | Some(answeringMessageFile) =>
                                         Some(
                                           answeringMessageFile.compressedFile.
                                             downloadUrl,
                                         )
                                       }
                                     }
                                     userCity={randomUser.city}
                                     distance={randomUser.distance}
                                     userId={randomUser.id}
                                     userFirstName={randomUser.firstName}
                                     onNopeClick={
                                       _mouseEvent =>
                                         mutation(
                                           ~variables=hideUser##variables,
                                           ~refetchQueries=[|"Discover"|],
                                           (),
                                         )
                                         |> ignore
                                     }
                                   />
                               )
                          </HideUserMutation>
                        </div>;
                      })
                   |> ReasonReact.array;
                 }
               </div>
             }
         }
    </DiscoverQuery>,
};
