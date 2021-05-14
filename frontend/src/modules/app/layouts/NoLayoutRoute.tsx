/**
 * @summary Layout with full empty container
 *
 */
import React from 'react';
import { Route, RouteProps } from 'react-router-dom';

/**
 * NoLayoutRoute
 *
 * @component Component for empty layout
 *
 * @returns {JSX.Element | null}
 */
const NoLayoutRoute = ({ component: Component, ...rest }: RouteProps): JSX.Element | null => {
  if (!Component) return null;
  return <Route {...rest} render={(matchProps) => <Component {...matchProps} />} />;
};

export default NoLayoutRoute;
