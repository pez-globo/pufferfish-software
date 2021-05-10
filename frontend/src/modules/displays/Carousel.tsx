/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 */
import { makeStyles } from '@material-ui/core';
import React, { ReactElement, useState } from 'react';
import ArrowForwardIosIcon from '@material-ui/icons/ArrowForwardIos';
import ArrowBackIosIcon from '@material-ui/icons/ArrowBackIos';

const useStyles = makeStyles(() => ({
  carousel: {
    position: 'relative',
    height: '100%',
  },
  carouselFlex: {
    maxWidth: '750px',
    display: 'flex',
    justifyContent: 'center',
    flexDirection: 'column',
    margin: '0 auto',
    height: '100%',

    '& ul': {
      padding: '0px',
    },
  },
  carouselIndicators: {
    display: 'flex',
    flexDirection: 'row',
    justifyContent: 'center',
    marginTop: '20px',
    listStyleType: 'none',
    '& li': {
      '& :nth-of-type(n + 2)': {
        marginLeft: '9px',
      },
    },
  },
  carouselArrow: {
    position: 'absolute',
    top: '50%',
    display: 'block',
    color: '#fff',
    cursor: 'pointer',
    opacity: '0.75',
    transform: 'translateY(-50%)',
    transition: 'opacity 0.15s cubic-bezier(0.4, 0, 1, 1)',
    '& :focus': {
      outline: 0,
    },
    '& :hover': {
      opacity: 0.5,
    },
  },
  carouselArrowRight: {
    right: '10%',
    border: '1px solid #fff',
    width: '45px',
    height: '45px',
    borderRadius: '50px',
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',

    '& svg': {
      paddingLeft: '6px',
    },
  },
  carouselArrowLeft: {
    left: '10%',
    border: '1px solid #fff',
    width: '45px',
    height: '45px',
    borderRadius: '50px',
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',

    '& svg': {
      paddingLeft: '6px',
    },
  },

  carouselIndicator: {
    display: 'block',
    width: '7px',
    height: '7px',
    backgroundColor: 'transparent',
    cursor: 'pointer',
    opacity: 0.75,
    transition: 'opacity 0.15s cubic-bezier(0.4, 0, 1, 1)',
    borderRadius: '50px',
    marginRight: '17px',
    border: '1px solid #fff',
    outline: 'none',
    listStyleType: 'none',

    '& :hover': {
      opacity: 0.5,
    },
  },
  carouselIndicatorActive: {
    opacity: 1,
    '& :hover': {
      opacity: 1,
    },
  },
  content: {
    marginBottom: '19px',
    fontSize: '16px',
  },

  carouselSlide: {
    marginRight: 'auto',
    marginLeft: 'auto',
    display: 'none',
    listStyleType: 'none',
    textAlign: 'center',
    background: '#0b2e4c',
    padding: '20px',
    borderRadius: '10px',
    minHeight: '350px',
  },
  carouselSlideActive: {
    display: 'block',
  },
}));

/**
 * CarouselLeftArrow
 *
 * @component A container for displaying the left arrow of carousel.
 *
 * @param {function} onClick - some desc for onClick
 *
 * @returns {JSX.Element}
 */
const CarouselLeftArrow = ({ onClick }: { onClick(): void }): JSX.Element => {
  const classes = useStyles();

  return (
    <div
      role="button"
      tabIndex={0}
      aria-label="Left"
      onKeyDown={() => null}
      className={`${classes.carouselArrow} ${classes.carouselArrowLeft}`}
      onClick={onClick}
    >
      <ArrowBackIosIcon />
    </div>
  );
};

/**
 * CarouselRightArrow
 *
 * @component A container for displaying the right arrow of carousel.
 *
 * @param {function} onClick - some desc for onClick
 *
 * @returns {JSX.Element}
 */
const CarouselRightArrow = ({ onClick }: { onClick(): void }): JSX.Element => {
  const classes = useStyles();

  return (
    <div
      role="button"
      tabIndex={0}
      aria-label="Right"
      onKeyDown={() => null}
      className={`${classes.carouselArrow} ${classes.carouselArrowRight}`}
      onClick={onClick}
    >
      <ArrowForwardIosIcon />
    </div>
  );
};

/**
 * CarouselIndicator
 *
 * @component A container for displaying the indicators of carousel.
 *
 * @param {function} onClick - some desc for onClick
 * @param {number} activeIndex - some desc for activeIndex
 * @param {number} index - some desc for index
 *
 * @returns {JSX.Element}
 */
const CarouselIndicator = ({
  onClick,
  activeIndex,
  index,
}: {
  onClick(): void;
  activeIndex: number;
  index: number;
}): JSX.Element => {
  const classes = useStyles();

  return (
    <li>
      <div
        role="button"
        tabIndex={0}
        aria-label="Indicator"
        onKeyDown={() => null}
        className={
          index === activeIndex
            ? `${classes.carouselIndicator} ${classes.carouselIndicatorActive}`
            : classes.carouselIndicator
        }
        onClick={onClick}
      />
    </li>
  );
};

/**
 * CarouselSlide
 *
 * @component A container for displaying the slides of carousel.
 *
 * @param {ReactElement} slide - some desc for slide
 * @param {number} activeIndex - some desc for activeIndex
 * @param {number} index - some desc for index
 *
 * @returns {JSX.Element}
 */
const CarouselSlide = ({
  slide,
  activeIndex,
  index,
}: {
  slide: ReactElement;
  activeIndex: number;
  index: number;
}): JSX.Element => {
  const classes = useStyles();

  return (
    <li
      className={
        index === activeIndex
          ? `${classes.carouselSlide} ${classes.carouselSlideActive}`
          : classes.carouselSlide
      }
    >
      <p className={classes.content}>{slide}</p>
    </li>
  );
};

// Carousel wrapper component
/**
 * Carousel
 *
 * @component A container for displaying the carousel wrapper.
 *
 * @param {ReactElement[]} slides - some desc for slides
 *
 * @returns {JSX.Element}
 */
export const Carousel = ({ slides }: { slides: ReactElement[] }): JSX.Element => {
  const classes = useStyles();
  const [activeIndex, setActiveIndex] = useState(0);

  /**
   * Goes to the previous slide
   */
  const goToPrevSlide = () => {
    let index = activeIndex;
    const slidesLength = slides.length;
    if (index < 1) {
      index = slidesLength;
    }
    index -= 1;
    setActiveIndex(index);
  };

  /**
   * Goes to the next slide
   */
  const goToNextSlide = () => {
    let index = activeIndex;
    const slidesLength = slides.length - 1;

    if (index === slidesLength) {
      index = -1;
    }
    index += 1;
    setActiveIndex(index);
  };

  return (
    <div className={classes.carousel}>
      <div className={classes.carouselFlex}>
        <CarouselLeftArrow onClick={() => goToPrevSlide()} />

        <ul>
          {slides.map((slide: ReactElement, index: number) => (
            <CarouselSlide
              key={`slide-${slide}`}
              index={index}
              activeIndex={activeIndex}
              slide={slide}
            />
          ))}
        </ul>

        <CarouselRightArrow onClick={() => goToNextSlide()} />

        <ul className={classes.carouselIndicators}>
          {slides.map((slide: ReactElement, index: number) => (
            <CarouselIndicator
              key={`indicator-${slide}`}
              index={index}
              activeIndex={activeIndex}
              onClick={() => setActiveIndex(index)}
            />
          ))}
        </ul>
      </div>
    </div>
  );
};

export default Carousel;
