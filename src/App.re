let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _ =>
    <ReasonApollo.Provider client=ApolloClient.instance>
      <Router />
    </ReasonApollo.Provider>,
};
