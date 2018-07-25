type file = {id: string};

type location = {
  latitude: float,
  longitude: float,
};

type messageFile = {
  id: string,
  emojiResume: option(string),
};

type user = {
  id: string,
  firstName: string,
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
            limit: 3
            offset: 0
          }
        ) @bsRecord {
          id
          firstName
          answeringMessageFile @bsRecord {
            id
            emojiResume(length: 3)
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
                        <div key=randomUser.id>
                          <h1>
                            (randomUser.firstName |> ReasonReact.string)
                          </h1>
                          (
                            switch (randomUser.answeringMessageFile) {
                            | None =>
                              "Pas d'enregistrement" |> ReasonReact.string
                            | Some(recording) =>
                              <div>
                                <h1>
                                  (
                                    switch (recording.emojiResume) {
                                    | None =>
                                      "Pas de resume" |> ReasonReact.string
                                    | Some(emojiResume) =>
                                      emojiResume |> ReasonReact.string
                                    }
                                  )
                                </h1>
                              </div>
                            }
                          )
                        </div>
                      )
                   |> ReasonReact.array;
                 }
               </div>
             }
         )
    </Query>,
};
