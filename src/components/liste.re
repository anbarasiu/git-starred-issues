let styles = Css.({
  "list": style([
    border(px(1),Solid,black),
    borderRadius(px(10)),
    padding(px(0)),
    margin(px(8)),
    width(pct(100.0)),
    media("(min-width: 678px)", [
      width(pct(47.0))
    ])
  ])
});

let component = ReasonReact.statelessComponent("Liste");

let make = (children) => {
  {
  ...component,
  render: (_self) => <ul className=styles##list> {ReasonReact.arrayToElement(children)} </ul>
  }
};