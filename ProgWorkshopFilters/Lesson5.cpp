/*
 * Your License or Copyright can go here
 */

#include "Lesson5.h"

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/partitioner.h>
#include <tbb/task_scheduler_init.h>
#endif

#include "SIMPLib/Common/Constants.h"

#include "SIMPLib/FilterParameters/DataArrayCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"

#include "ProgWorkshop/ProgWorkshopConstants.h"
#include "ProgWorkshop/ProgWorkshopVersion.h"

/**
 * @brief The Lesson5Impl class implements a threaded algorithm that computes the IPF
 * colors for each element in a geometry
 */
class Lesson5Impl
{
public:
  Lesson5Impl(float* input, float* output, int operation, float value, bool selection, float fValue)
  : m_Input(input)
  , m_Output(output)
  , m_Operation(operation)
  , m_Value(value)
  , m_Selection(selection)
  , m_FValue(fValue)
  {
  }
  virtual ~Lesson5Impl()
  {
  }

  void convert(size_t start, size_t end) const
  {
    if(m_Operation == 0)
    {
      for(size_t i = start; i < end; i++)
      {
        m_Output[i] = m_Input[i] * m_Value;
      }
    }
    else if(m_Operation == 1)
    {
      for(size_t i = start; i < end; i++)
      {
        m_Output[i] = m_Input[i] / m_Value;
      }
    }
    else if(m_Operation == 2)
    {
      for(size_t i = start; i < end; i++)
      {
        m_Output[i] = m_Input[i] + m_Value;
      }
    }
    else if (m_Operation == 3)
    {
      for(size_t i = start; i < end; i++)
      {
        m_Output[i] = m_Input[i] - m_Value;
      }
    }

    // Use the LinkedBoolean in a calculation
    if(m_Selection)
    {
      for(size_t i = start; i < end; i++)
      {
        m_Output[i] = m_Output[i] - m_FValue;
      }
    }
  }

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
  void operator()(const tbb::blocked_range<size_t>& r) const
  {
    convert(r.begin(), r.end());
  }
#endif
private:
  float* m_Input;
  float* m_Output;
  int m_Operation;
  float m_Value;
  bool m_Selection;
  float m_FValue;

};


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Lesson5::Lesson5()
: // Initialize all your filter parameters here.  SIMPL has a Constants file that developers can use to help set common names.
    m_InputDataArrayPath(SIMPL::Defaults::ImageDataContainerName, SIMPL::Defaults::CellAttributeMatrixName, SIMPL::CellData::ImageQuality)
, m_OutputDataArrayPath(SIMPL::Defaults::ImageDataContainerName, SIMPL::Defaults::CellAttributeMatrixName, "IQ_Mod")
, m_Value(12342.1f)
, m_Operator(1)
, m_Selection(false)
, m_FloatValue(128.128f)
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Lesson5::~Lesson5() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Lesson5::initialize()
{
  setErrorCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Lesson5::setupFilterParameters()
{
  FilterParameterVector parameters;
  //-- Add a Requirement to the input array that is be of type float and 1 component
  DataArraySelectionFilterParameter::RequirementType dasReq = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::Float, 1, AttributeMatrix::Category::Any);
  parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("InputDataArrayPath", InputDataArrayPath, FilterParameter::RequiredArray, Lesson5, dasReq));
  DataArrayCreationFilterParameter::RequirementType dacReq;
  parameters.push_back(SIMPL_NEW_DA_CREATION_FP("OutputDataArrayPath", OutputDataArrayPath, FilterParameter::CreatedArray, Lesson5, dacReq));
  parameters.push_back(SIMPL_NEW_FLOAT_FP("Value", Value, FilterParameter::Parameter, Lesson5));

  // When adding a Choice FilterParameter you need to define the choices first
  // What goes into the choices can be statically coded (Like below) or dynamically
  // computed based on something. EMsoft has an example of this.
  {
    QVector<QString> choices;
    choices.push_back("*");
    choices.push_back("/");
    choices.push_back("+");
    choices.push_back("-");
    // Create the Choice Filter Parameter and add it to the list of parameters
    parameters.push_back(SIMPL_NEW_CHOICE_FP("Operator", Operator, FilterParameter::Parameter, Lesson5, choices, false));
  }

  QStringList linkedProps("FloatValue");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Linked Bool", Selection, FilterParameter::Parameter, Lesson5, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_FP("Float Value", FloatValue, FilterParameter::Parameter, Lesson5));


  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Lesson5::dataCheck()
{
  // Reset the error condition to ZERO
  setErrorCondition(0);

  /* This method should be used to sanity all the filter parameters to make sure
   * they meet your expectations
   */

  //-- Validate the Input Array
  // Be sure to add  DEFINE_DATAARRAY_VARIABLE(float, InputData)  in the header in the private section
  // m_InputData is a **RAW** pointer which is both dangerous and fast
  // An error message will be broadcasted if there are any errors
  QVector<size_t> cDims = {1}; // Single component Array
  m_InputDataPtr = getDataContainerArray()->getPrereqArrayFromPath<FloatArrayType, AbstractFilter>(this, getInputDataArrayPath(), cDims);
  /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  if(nullptr != m_InputDataPtr.lock())
  {
    m_InputData = m_InputDataPtr.lock()->getPointer(0);/* Now assign the raw pointer to data from the DataArray<T> object */
  }

  //-- Validate the Output Array
  // Be sure to add  DEFINE_DATAARRAY_VARIABLE(float, OutputArray)  in the header in the private section
  cDims[0] = 1; // This is not necessary because we are creating a Single Component Array
  m_OutputDataPtr = getDataContainerArray()->createNonPrereqArrayFromPath<FloatArrayType, AbstractFilter, float>(this, getOutputDataArrayPath(), 0, cDims);
  /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  if(nullptr != m_OutputDataPtr.lock())
  {
    m_OutputData = m_OutputDataPtr.lock()->getPointer(0);/* Now assign the raw pointer to data from the DataArray<T> object */
  }

  if(getValue() < 0.0f)
  {
    setErrorCondition(-10000);
    notifyErrorMessage(getHumanLabel(), "Float value must be greater than 0.0", getErrorCondition());
  }

  if(getOperator() < 0 || getOperator() > 3)
  {
    setErrorCondition(-10001);
    notifyErrorMessage(getHumanLabel(), "The operator can only have values of 0,1,2,3", getErrorCondition());
  }

  if(getSelection())
  {
    if(getFloatValue() < 100 || getFloatValue() > 200)
    {
      setErrorCondition(-10002);
      notifyErrorMessage(getHumanLabel(), "FloatValue should be in the range of 100 to 200", getErrorCondition());
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Lesson5::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true);              // Set the fact that we are preflighting.
  emit preflightAboutToExecute();    // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck();                       // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted();          // We are done preflighting this filter
  setInPreflight(false);             // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Lesson5::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  // If your filters are going to take a while to run you should check for the
  // pipeline being canceled. You could potentially put this into the loop below
  // to check every iteration if you wanted to.
  if(getCancel())
  {
    return;
  }


  // Loop on every Tuple in the InputArray, mulitply by the Value and place in the
  // OutputArray. There are already several variables and raw pointers setup for
  // you in the header that we can use.
  /****************************************************************************/
  // This is the Raw Pointer Version of the loop. It will be fast, but dangerous
  // because there is NO bounds checking when setting the value. If you walk off
  // the end of the array your program will likely crash or just give bad results.
  /****************************************************************************/
  size_t numTuples = m_InputDataPtr.lock()->getNumberOfTuples();

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
  tbb::task_scheduler_init init;
  bool doParallel = true;
#endif

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
  if(doParallel == true)
  {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, numTuples), Lesson5Impl(m_InputData, m_OutputData, m_Operator, m_Value, m_Selection, m_FloatValue),
                      tbb::auto_partitioner());
  }
  else
#endif
  {
    Lesson5Impl serial(m_InputData, m_OutputData, m_Operator, m_Value, m_Selection, m_FloatValue);
    serial.convert(0, numTuples);
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer Lesson5::newFilterInstance(bool copyFilterParameters) const
{
  Lesson5::Pointer filter = Lesson5::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Lesson5::getCompiledLibraryName() const
{
  return ProgWorkshopConstants::ProgWorkshopBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Lesson5::getBrandingString() const
{
  return "ProgWorkshop";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Lesson5::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ProgWorkshop::Version::Major() << "." << ProgWorkshop::Version::Minor() << "." << ProgWorkshop::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Lesson5::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid Lesson5::getUuid()
{
  return QUuid("{169f34c2-62b2-57cf-ac79-9067dbac0d73}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Lesson5::getSubGroupName() const
{
  return "ProgWorkshop";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Lesson5::getHumanLabel() const
{
  return "Lesson5";
}
