type file = {id: string};

type location = {
  latitude: float,
  longitude: float,
};

type me = {
  firstName: string,
  answeringMessageFile: option(file),
  location: option(location),
};

module Home = [%graphql
  {|
    query home {
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

module HomeQuery = ReasonApollo.CreateQuery(Home);

type state = {
  skipRecordingUpload: bool,
  skipLocation: bool,
};
type action =
  | SkipRecordingUpload
  | SkipLocation;

module HomeData = {
  let component = ReasonReact.reducerComponent("HomeData");
  let make = (~me: me, _children) => {
    ...component,
    reducer: (action: action, state: state) =>
      switch (action) {
      | SkipRecordingUpload =>
        ReasonReact.Update({...state, skipRecordingUpload: true})
      | SkipLocation => ReasonReact.Update({...state, skipLocation: true})
      },
    initialState: () => {skipRecordingUpload: false, skipLocation: false},
    render: self => {
      let {firstName, answeringMessageFile, location} = me;
      switch (
        self.state.skipRecordingUpload,
        answeringMessageFile,
        self.state.skipLocation,
        location,
      ) {
      | (false, None, _, _) =>
        <RecordingUploadUnboarding
          firstName
          onSkip=(_ => self.send(SkipRecordingUpload))
        />
      | (_, _, false, None) =>
        <LocationUploadUnboarding onSkip=(_ => self.send(SkipLocation)) />
      | _ =>
        <div>
          (ReasonReact.string("This is supposed to be the home page"))
        </div>
      };
    },
  };
};

let component = ReasonReact.statelessComponent("HomePage");

let make = _children => {
  ...component,
  render: _self =>
    <HomeQuery>
      ...(
           ({result}) =>
             switch (result) {
             | Loading => <LoadingSection />
             | Error(error) => <ErrorSection errorMessage=error##message />
             | Data(data) => <HomeData me=data##me />
             }
         )
    </HomeQuery>,
};
