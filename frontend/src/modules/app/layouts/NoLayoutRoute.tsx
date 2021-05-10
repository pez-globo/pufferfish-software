/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
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
