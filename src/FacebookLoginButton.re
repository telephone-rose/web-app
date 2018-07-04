type session = {
  authToken: string,
  refreshToken: string,
};

module LoginUsingFacebook = [%graphql
  {|
    mutation loginUsingFacebook ($facebookToken: String!) {
      loginUsingFacebook (facebookToken: $facebookToken) @bsRecord {
        authToken
        refreshToken
      }
    }
  |}
];

module LoginUsingFacebookMutation =
  ReasonApollo.CreateMutation(LoginUsingFacebook);

module FacebookLoginButton = {
  type facebookAccessToken = string;

  type action =
    | InitTelephoneRoseLogin(facebookAccessToken)
    | InitLoginButton
    | TriggerFacebookLogin
    | DisplayLoginError;

  type state = {
    loading: bool,
    loginError: bool,
  };

  let component = ReasonReact.reducerComponent("FacebookLoginButton");

  let make =
      (
        ~mutate: LoginUsingFacebookMutation.apolloMutation,
        ~onLogin,
        _children,
      ) => {
    ...component,
    initialState: () => {loading: true, loginError: false},
    didMount: self =>
      FbSdk.getLoginStatus(loginStatus =>
        switch (loginStatus) {
        | Connected(facebookCredentials) =>
          self.send(InitTelephoneRoseLogin(facebookCredentials.accessToken))
        | _ => self.send(InitLoginButton)
        }
      ),
    reducer: (action: action, state: state) =>
      switch (action) {
      | InitLoginButton => ReasonReact.Update({...state, loading: false})
      | DisplayLoginError =>
        ReasonReact.Update({loading: false, loginError: true})
      | TriggerFacebookLogin =>
        ReasonReact.UpdateWithSideEffects(
          {loading: true, loginError: false},
          (
            self =>
              FbSdk.login(
                ~scopes=[|"email"|],
                authResult => {
                  switch (authResult) {
                  | Connected(authResult) =>
                    self.send(InitTelephoneRoseLogin(authResult.accessToken))
                  | _ => self.send(DisplayLoginError)
                  };
                  self.send(DisplayLoginError);
                },
              )
          ),
        )
      | InitTelephoneRoseLogin(facebookAccessToken) =>
        ReasonReact.UpdateWithSideEffects(
          {loading: true, loginError: false},
          (
            self => {
              let facebookCredentials =
                LoginUsingFacebook.make(
                  ~facebookToken=facebookAccessToken,
                  (),
                );
              Js.Promise.(
                mutate(~variables=facebookCredentials##variables, ())
                |> then_(response =>
                     resolve(
                       LoginUsingFacebookMutation.convertJsInputToReason(
                         response,
                       ),
                     )
                   )
                |> then_((response: LoginUsingFacebookMutation.renderPropObj) => {
                     switch (response.data) {
                     | Some(data) =>
                       let root = data##loginUsingFacebook;
                       onLogin(root.authToken, root.refreshToken);
                       ();
                     | _ => self.send(DisplayLoginError)
                     };
                     resolve();
                   })
                |> ignore
              );
            }
          ),
        )
      },
    render: self =>
      <button
        onClick=(_ => self.send(TriggerFacebookLogin))
        className=(
          "button is-medium is-light"
          ++ (self.state.loading ? " is-loading" : "")
        )>
        <span className="icon"> <i className="fab fa-facebook" /> </span>
        <span> (ReasonReact.string("Facebook")) </span>
      </button>,
  };
};

let component = ReasonReact.statelessComponent("FacebookLoginButtonWrapper");

let make = (~onLogin, _) => {
  ...component,
  render: _ =>
    <LoginUsingFacebookMutation>
      ...((mutate, _) => <FacebookLoginButton onLogin mutate />)
    </LoginUsingFacebookMutation>,
};
