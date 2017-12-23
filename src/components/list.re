let styles = Css.({
  "list": style([
    border(px(1),Solid,black),
    borderRadius(px(10)),
    padding(px(0)),
    margin(px(8))
  ])
});

let component = ReasonReact.statelessComponent("List");

let make = (children) => {
  {
  ...component,
  render: (_self) => <ul className=styles##list> {ReasonReact.arrayToElement(children)} </ul>
  }
};