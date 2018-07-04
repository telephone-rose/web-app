type state = {authToken: option(string)};

type action =
  | SetAuthToken(string, string)
  | UnsetAuthToken;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {authToken: None},
  reducer: (action, _) =>
    switch (action) {
    | SetAuthToken(authToken, _) =>
      ReasonReact.Update({authToken: Some(authToken)})
    | UnsetAuthToken => ReasonReact.Update({authToken: None})
    },
  render: self =>
    <ReasonApollo.Provider
      client=(ApolloClient.makeInstance(self.state.authToken))>
      <Router
        isAuthenticated=(self.state.authToken !== None)
        onLogin=(
          (authToken, refreshToken) =>
            self.send(SetAuthToken(authToken, refreshToken))
        )
        /* onLogout=(() => self.send(UnsetAuthToken))

           */
      />
    </ReasonApollo.Provider>,
};
