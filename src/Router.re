type page =
  | Home
  | NotFound;

type action =
  | ShowHome
  | ShowNotFound;

type state = {page};

let component = ReasonReact.reducerComponent("Router");

let route = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => ShowHome
  | _ => ShowNotFound
  };

let make = (~isAuthenticated, ~onLogin, _children) => {
  ...component,
  initialState: () => {page: Home},
  reducer: (action, _) =>
    switch (action) {
    | ShowHome => ReasonReact.Update({page: Home})
    | ShowNotFound => ReasonReact.Update({page: NotFound})
    },
  didMount: self => {
    self.send(route(ReasonReact.Router.dangerouslyGetInitialUrl()));
    let watcherID =
      ReasonReact.Router.watchUrl(url => self.send(route(url)));
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
    ();
  },
  render: self =>
    switch (isAuthenticated, self.state.page) {
    | (_, NotFound) => <NotFoundPage />
    | (false, _) => <LoginPage onLogin />
    | (_, Home) => <HomePage />
    },
};
