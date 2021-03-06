/*
 * Your License or Copyright can go here
 */

#pragma once

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "ProgWorkshop/ProgWorkshopDLLExport.h"

/**
 * @brief The Lesson3 class. See [Filter documentation](@ref lesson3) for details.
 */
class ProgWorkshop_EXPORT Lesson3 : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(Lesson3 SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(DataArrayPath InputDataArrayPath READ getInputDataArrayPath WRITE setInputDataArrayPath)
  PYB11_PROPERTY(DataArrayPath OutputDataArrayPath READ getOutputDataArrayPath WRITE setOutputDataArrayPath)
  PYB11_PROPERTY(float Value READ getValue WRITE setValue)
  PYB11_PROPERTY(int Operator READ getOperator WRITE setOperator)

public:
  SIMPL_SHARED_POINTERS(Lesson3)
  SIMPL_FILTER_NEW_MACRO(Lesson3)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(Lesson3, AbstractFilter)

  ~Lesson3() override;

  SIMPL_FILTER_PARAMETER(DataArrayPath, InputDataArrayPath)
  Q_PROPERTY(DataArrayPath InputDataArrayPath READ getInputDataArrayPath WRITE setInputDataArrayPath)

  SIMPL_FILTER_PARAMETER(DataArrayPath, OutputDataArrayPath)
  Q_PROPERTY(DataArrayPath OutputDataArrayPath READ getOutputDataArrayPath WRITE setOutputDataArrayPath)

  SIMPL_FILTER_PARAMETER(float, Value)
  Q_PROPERTY(float Value READ getValue WRITE setValue)

  SIMPL_FILTER_PARAMETER(int, Operator)
  Q_PROPERTY(int Operator READ getOperator WRITE setOperator)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  const QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  const QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  const QString getFilterVersion() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  const QUuid getUuid() override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  const QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
   * @brief preflight Reimplemented from @see AbstractFilter class
   */
  void preflight() override;

signals:
  /**
   * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
   * be pushed from a user-facing control (such as a widget)
   * @param filter Filter instance pointer
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged Emitted when any Filter parameter is changed internally
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before calling dataCheck()
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after calling dataCheck()
   */
  void preflightExecuted();

protected:
  Lesson3();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  DEFINE_DATAARRAY_VARIABLE(float, InputData)
  DEFINE_DATAARRAY_VARIABLE(float, OutputData)

public:
  Lesson3(const Lesson3&) = delete;            // Copy Constructor Not Implemented
  Lesson3(Lesson3&&) = delete;                 // Move Constructor Not Implemented
  Lesson3& operator=(const Lesson3&) = delete; // Copy Assignment Not Implemented
  Lesson3& operator=(Lesson3&&) = delete;      // Move Assignment
};

