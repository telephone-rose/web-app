type session = {
  authToken: string,
  refreshToken: string,
};

module LoginUsingGoogle = [%graphql
  {|
    mutation loginUsingGoogle ($googleIdToken: String!) {
      loginUsingGoogle (googleIdToken: $googleIdToken) @bsRecord {
        authToken
        refreshToken
      }
    }
  |}
];

module LoginUsingGoogleMutation =
  ReasonApollo.CreateMutation(LoginUsingGoogle);

module GoogleLoginButton = {
  type googleAccessToken = string;

  type action =
    | InitTelephoneRoseLogin(googleAccessToken)
    | InitLoginButton(GoogleSdk.googleAuth)
    | TriggerGoogleLogin(GoogleSdk.googleAuth)
    | DisplayLoginError;

  type state = {
    loading: bool,
    loginError: bool,
    googleAuth: option(GoogleSdk.googleAuth),
  };

  let component = ReasonReact.reducerComponent("GoogleLoginButton");

  let make =
      (~mutate: LoginUsingGoogleMutation.apolloMutation, ~onLogin, _children) => {
    ...component,
    initialState: () => {loading: true, loginError: false, googleAuth: None},
    didMount: self => {
      Js.Promise.(
        GoogleSdk.googleAuthPromise
        |> Js.Promise.then_(googleAuth => {
             ();
             self.send(InitLoginButton(googleAuth));
             resolve();
           })
      )
      |> ignore;
      ();
    },
    reducer: (action: action, state: state) =>
      switch (action) {
      | InitLoginButton(googleAuth) =>
        ReasonReact.Update({
          ...state,
          googleAuth: Some(googleAuth),
          loading: false,
        })
      | DisplayLoginError =>
        ReasonReact.Update({...state, loading: false, loginError: true})
      | TriggerGoogleLogin(googleAuth) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, loading: true, loginError: false},
          (
            self =>
              googleAuth
              |> GoogleSdk.signIn
              |> Js.Promise.then_(token => {
                   self.send(InitTelephoneRoseLogin(token));
                   Js.Promise.resolve();
                 })
              |> ignore
          ),
        )
      | InitTelephoneRoseLogin(googleAccessToken) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, loading: true, loginError: false},
          (
            self => {
              let facebookCredentials =
                LoginUsingGoogle.make(~googleIdToken=googleAccessToken, ());
              Js.Promise.(
                mutate(~variables=facebookCredentials##variables, ())
                |> then_(response =>
                     resolve(
                       LoginUsingGoogleMutation.convertJsInputToReason(
                         response,
                       ),
                     )
                   )
                |> then_((response: LoginUsingGoogleMutation.renderPropObj) => {
                     switch (response.data) {
                     | Some(data) =>
                       let root = data##loginUsingGoogle;
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
      switch (self.state.googleAuth) {
      | None =>
        <button
          disabled=true
          className=(
            "button is-medium is-light"
            ++ (self.state.loading ? " is-loading" : "")
          )>
          <span className="icon"> <i className="fab fa-google" /> </span>
          <span> (ReasonReact.string("Google")) </span>
        </button>
      | Some(googleAuth) =>
        <button
          onClick=(_ => self.send(TriggerGoogleLogin(googleAuth)))
          className=(
            "button is-medium is-light"
            ++ (self.state.loading ? " is-loading" : "")
          )>
          <span className="icon"> <i className="fab fa-google" /> </span>
          <span> (ReasonReact.string("Google")) </span>
        </button>
      },
  };
};

let component = ReasonReact.statelessComponent("FacebookLoginButtonWrapper");

let make = (~onLogin, _) => {
  ...component,
  render: _ =>
    <LoginUsingGoogleMutation>
      ...((mutate, _) => <GoogleLoginButton onLogin mutate />)
    </LoginUsingGoogleMutation>,
};
