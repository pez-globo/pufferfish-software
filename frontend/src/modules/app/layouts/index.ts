/**
 * @summary Components comprising of all Layout files
 * These Layout files are used in `Route.tsx` file
 * to define which layout to use for which page route
 *
 * @file Layout file contains wrapper for Sidebar & Toolbar
 * To create new customized layout will require
 * to add a file in this directory with customized changes
 *
 * To use the layout for a page, Define Layout while declaring page route, Example below
 * `SidebarSlideRoute` layout can be used by `NewPage` using following example
 * `<SidebarSlideRoute exact path={NewRoutePath} component={NewPage} />`
 */
export * from './LandingPageRoute';
export * from './NoLayoutRoute';
export * from './ScreensaverRoute';
export * from './SidebarRoute';
export * from './SidebarSlideRoute';
